/**
 * A boids implementation based on [Boids]
 * (http://www.red3d.com/cwr/boids/) by Craig Reynolds,
 * and the course material.
 *
 * @author Dennis Kristiansen
 * @file boids.cpp
 * @date 1.2.2020
 */

#include "common.h"

// Constants
// ***********************************************************************

constexpr uint32_t WINDOWX     = 1000;
constexpr uint32_t WINDOWY     = 1000;
constexpr uint32_t NUM_OBJECTS = 100;
constexpr float    MAX_ANGLE   = 6.0f * M_PI_4;

// Globals
// ***********************************************************************

std::vector<class Boid *>     gBoids;
std::vector<class Predator *> gPredators;
std::function<float()>        rnd;

// class declarations
// ***********************************************************************

class Boid {
  public:
	Boid();
	virtual ~Boid() = default;
	virtual void update(float dt);
	void         draw(sf::RenderWindow &w);
	sf::Vector2f getPosition() const;
	sf::Vector2f getVelocity() const;

  protected:
	sf::Vector2f computeCohesion();
	sf::Vector2f computeSeparation();
	sf::Vector2f computeAlignment();
	sf::Vector2f computeFlee();
	sf::Vector2f steer(sf::Vector2f dir, float steerforce);

	static constexpr float maxspeed     = 80.0f; //< Max speed of the boid
	static constexpr float maxforce     = 1.0f;  //< Max force to apply per rule
	static constexpr float cohesiondist = 200.0f;   //< Max cohesion distance
	static constexpr float separationdist = 80.0f;  //< Max separation distance
	static constexpr float alignmentdist  = 220.0f; //< Max alignment distance
	static constexpr float fleedist       = 300.0f; //< Max flee distance

	sf::Vector2f    pos;
	sf::Vector2f    vel;
	sf::CircleShape shape;
};

/**
 * A Predator that chases boids, but never catches them.
 */
class Predator : public Boid {
  public:
	Predator();
	void update(float dt) override;
};

// Helper functions
// ***********************************************************************

/**
 * Computes whether or not a boid is visible from another.
 * This function only considers visibility based on angle.
 *
 * @param a The first boid.
 * @param b The second boid.
 * @return  Is the boid b visible from boid a?
 */
bool visible(const Boid *a, const Boid *b) {
	auto vel     = a->getVelocity();
	auto a_angle = atan2(vel.y, vel.x);

	auto diff    = b->getPosition() - a->getPosition();
	auto b_angle = atan2(diff.y, diff.x);

	auto angle = b_angle - a_angle;

	return angle < MAX_ANGLE && angle > -MAX_ANGLE;
}

// Boid impl
// ***********************************************************************

/**
 * Initializes the boid with a random position and velocity.
 */
Boid::Boid() {
	pos        = sf::Vector2f(rnd() * WINDOWX / 2.0f, rnd() * WINDOWY / 2.0f);
	auto angle = (rnd() + 1.0f) * M_PI;
	vel        = sf::Vector2f(maxspeed * cos(angle), maxspeed * sin(angle));
	shape      = sf::CircleShape(10.0f, 3);
	shape.setPosition(pos - sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setFillColor(sf::Color::Red);
	shape.setOrigin(5.0f, 5.0f);
}

/**
 * Update the internal state of the boid.
 *
 * @param dt Delta time, time since last update.
 */
void Boid::update(float dt) {
	auto v1 = computeCohesion();
	auto v2 = computeSeparation();
	auto v3 = computeAlignment();
	auto v4 = computeFlee();

	auto acc = 1.75f * v1 + 6.0f * v2 + 0.25f * v3 + 10.0f * v4;

	vel += acc;
	vel = maxspeed * vel / length(vel);
	pos += vel * dt;

	// Wrap around the screen
	if (pos.x > WINDOWX + shape.getRadius())
		pos.x -= WINDOWX + shape.getRadius();
	else if (pos.x < -shape.getRadius())
		pos.x += WINDOWX + shape.getRadius();

	if (pos.y > WINDOWY + shape.getRadius())
		pos.y -= WINDOWY + shape.getRadius();
	else if (pos.y < -shape.getRadius())
		pos.y += WINDOWY + shape.getRadius();
}

/**
 * Draw the boid.
 *
 * @param w The SFML window that does the actual drawing
 */
void Boid::draw(sf::RenderWindow &w) {
	// Set the position to rotate around
	shape.setPosition(pos);
	// 210 was found using experimentation
	shape.setRotation(atan2(vel.y, vel.x) / M_PI * 180.0f + 210.0f);

	// Set the actual position
	shape.setPosition(pos - sf::Vector2f(shape.getRadius(), shape.getRadius()));

	w.draw(shape);
}

sf::Vector2f Boid::getPosition() const { return pos; }

sf::Vector2f Boid::getVelocity() const { return vel; }

/**
 * Reynolds steering function.
 *
 * @param dir Desired direction.
 * @return    Direction to steer scaled to maxforce.
 *
 * @see [Steering Behaviors For Autonomous Characters]
 * (http://www.red3d.com/cwr/steer/gdc99/)
 */
sf::Vector2f Boid::steer(const sf::Vector2f dir, float steerforce) {
	auto ret = maxspeed * dir / length(dir) - vel;
	return limit(ret, steerforce);
}

/**
 * Compute the steering vector based on the cohesion rule.
 */
sf::Vector2f Boid::computeCohesion() {
	auto   ret     = sf::Vector2f(0.0f, 0.0f);
	size_t num_vis = 0;

	for (auto other : gBoids) {
		auto dist = length(other->getPosition() - pos);
		if (dist > 0.0f && dist < cohesiondist && visible(this, other)) {
			ret += other->getPosition();
			num_vis++;
		}
	}

	// Get the average vector
	ret /= (float)num_vis;

	// Avoid dividing by zero
	if (num_vis > 0)
		return steer(ret, maxforce);
	else
		return sf::Vector2f(0.0f, 0.0f);
}

/**
 * Compute the steering vector based on the separation rule.
 */
sf::Vector2f Boid::computeSeparation() {
	auto   ret     = sf::Vector2f(0.0f, 0.0f);
	size_t num_vis = 0;

	for (auto other : gBoids) {
		auto diff = pos - other->getPosition();
		auto dist = length(diff);
		if (dist > 0.0f && dist < separationdist && visible(this, other)) {
			// FIXME: This should scale vectors based on distance. Closer boids
			// should give greater reaction
			diff /= dist;
			diff /= dist;
			ret += diff;
			num_vis++;
		}
	}

	// Get the average vector
	ret /= (float)num_vis;

	if (num_vis > 0)
		return steer(ret, maxforce);
	else
		return sf::Vector2f(0.0f, 0.0f);
}

/**
 * Compute the steering vector based on the alignment rule.
 */
sf::Vector2f Boid::computeAlignment() {
	auto   ret     = sf::Vector2f(0.0f, 0.0f);
	size_t num_vis = 0;

	for (auto other : gBoids) {
		auto dist = length(pos - other->getPosition());
		if (dist > 0.0f && dist < alignmentdist && visible(this, other)) {
			ret += other->getVelocity();
			num_vis++;
		}
	}

	// Get the average vector
	ret /= (float)num_vis;

	if (num_vis > 0)
		return steer(ret, maxforce);
	else
		return sf::Vector2f(0.0f, 0.0f);
}

/**
 * Compute the steering vector based on fleeing rule, ie flee if near predator.
 */
sf::Vector2f Boid::computeFlee() {
	auto   ret     = sf::Vector2f(0.0f, 0.0f);
	size_t num_vis = 0;

	for (auto other : gPredators) {
		auto diff = pos - other->getPosition();
		auto dist = length(diff);
		if (dist < fleedist && visible(this, other)) {
			diff /= dist;
			ret += diff;
			num_vis++;
		}
	}

	// Get the average vector
	ret /= (float)num_vis;

	if (num_vis > 0) {
		return steer(ret, 2.0f);
	} else
		return sf::Vector2f(0.0f, 0.0f);
}

// ***********************************************************************

Predator::Predator() {
	pos        = sf::Vector2f(rnd() * WINDOWX / 2.0f, rnd() * WINDOWY / 2.0f);
	auto angle = (rnd() + 1.0f) * M_PI;
	vel        = sf::Vector2f(maxspeed * cos(angle), maxspeed * sin(angle));
	shape      = sf::CircleShape(20.0f, 3);
	shape.setPosition(pos - sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setFillColor(sf::Color::Yellow);
}

// TODO: Can I expose the weights instead of this
void Predator::update(float dt) {
	auto v1 = computeCohesion();

	vel += 2.0f * v1;
	vel = maxspeed * vel / length(vel);
	pos += vel * dt;

	// Wrap around the screen
	if (pos.x > WINDOWX + shape.getRadius())
		pos.x -= WINDOWX + shape.getRadius();
	else if (pos.x < -shape.getRadius())
		pos.x += WINDOWX + shape.getRadius();

	if (pos.y > WINDOWY + shape.getRadius())
		pos.y -= WINDOWY + shape.getRadius();
	else if (pos.y < -shape.getRadius())
		pos.y += WINDOWY + shape.getRadius();
}
// Main
// ***********************************************************************

int main() {
	// Init randomness
	// *******************************************************************
	std::random_device                    rd;
	std::default_random_engine            generator(rd());
	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
	rnd = std::bind(distribution, generator);

	// Create window
	sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY), "Perlin noise");
	window.setFramerateLimit(30);

	// Setup boids
	gBoids.resize(NUM_OBJECTS);
	for (size_t i = 0; i < NUM_OBJECTS; i++)
		gBoids[i] = new Boid();

	gPredators.resize(2);
	for (size_t i = 0; i < 2; i++)
		gPredators[i] = new Predator();

	sf::Clock clock;
	clock.restart();

	// Game loop
	// *******************************************************************
	while (window.isOpen()) {
		// Event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed: window.close(); break;

				default: break;
			}
		}

		float dt = clock.restart().asSeconds();

		// Update boids
		for (auto boid : gBoids)
			boid->update(dt);

		for (auto predator : gPredators)
			predator->update(dt);

		// Rendering
		window.clear();

		// Draw boids
		for (auto boid : gBoids)
			boid->draw(window);

		for (auto predator : gPredators)
			predator->draw(window);

		window.display();
	}

	// Cleanup
	for (auto boid : gBoids)
		delete boid;

	for (auto predator : gPredators)
		delete predator;

	gBoids.clear();
	gPredators.clear();

	return EXIT_SUCCESS;
}
