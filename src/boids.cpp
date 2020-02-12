/**
 * A boids implementation based on "Boids",
 * (http://www.red3d.com/cwr/boids/) by Reynolds,
 * and the course material.
 *
 * @author Dennis Kristiansen
 * @file boids.cpp
 * @date 1.2.2020
 */

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Constants
// ***********************************************************************

const unsigned int windowx  = 1000;
const unsigned int windowy  = 1000;
const unsigned int num_obj  = 100;
const float        maxangle = 6.0f * M_PI_4;

// Globals
// ***********************************************************************

std::vector<class Boid *>     gBoids;
std::vector<class Predator *> gPredators;
std::function<float()>        rnd;

// Boid decl
// ***********************************************************************

class Boid {
  public:
	Boid();
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

	const float maxspeed       = 80.0f;  //< Max speed of the boid
	const float maxforce       = 1.0f;   //< Max force to apply per rule
	const float cohesiondist   = 200.0f; //< Max cohesion distance
	const float separationdist = 80.0f;  //< Max separation distance
	const float alignmentdist  = 220.0f; //< Max alignment distance
	const float fleedist       = 300.0f; //< Max flee distance

	sf::Vector2f    pos;
	sf::Vector2f    vel;
	sf::CircleShape shape;
};

class Predator : public Boid {
  public:
	Predator();
	void update(float dt) override;
};

// Helper functions
// ***********************************************************************

/**
 * Returns the length of the given vector.
 */
float length(const sf::Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }

/**
 * Limit a vectors magnitude.
 *
 * @param v   - The vector
 * @param len - The max length of the vector
 * @return    - The vector now with a magnitude shorter or equaly to len
 */
sf::Vector2f limit(const sf::Vector2f v, float len) {
	if (length(v) > len) {
		return len * v / length(v);
	} else {
		return v;
	}
}

/**
 * Computes whether or not a boid is visible from another.
 * This function only considers visibility based on angle.
 *
 * @param a - The first boid.
 * @param b - The second boid.
 * @return  - Is the boid b visible from boid a?
 */
bool visible(const Boid *a, const Boid *b) {
	auto vel     = a->getVelocity();
	auto a_angle = atan2(vel.y, vel.x);

	auto diff    = b->getPosition() - a->getPosition();
	auto b_angle = atan2(diff.y, diff.x);

	auto angle = b_angle - a_angle;

	return angle < maxangle && angle > -maxangle;
}

// Boid impl
// ***********************************************************************

/**
 * Construct a boid
 *
 * Initializes the boid with a random position and velocity.
 */
Boid::Boid() {
	pos        = sf::Vector2f(rnd() * windowx / 2.0f, rnd() * windowy / 2.0f);
	auto angle = (rnd() + 1.0f) * M_PI;
	vel        = sf::Vector2f(maxspeed * cos(angle), maxspeed * sin(angle));
	shape      = sf::CircleShape(10.0f, 3);
	shape.setPosition(pos - sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setFillColor(sf::Color::Red);
}

/**
 * Update the internal state of the boid
 *
 * @param dt - Delta time, time since last update.
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
	if (pos.x > windowx + shape.getRadius())
		pos.x -= windowx + shape.getRadius();
	else if (pos.x < -shape.getRadius())
		pos.x += windowx + shape.getRadius();

	if (pos.y > windowy + shape.getRadius())
		pos.y -= windowy + shape.getRadius();
	else if (pos.y < -shape.getRadius())
		pos.y += windowy + shape.getRadius();
}

/**
 * Draw the boid.
 *
 * @param w - The SFML window that does the actual drawing
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
 * @param dir - Desired direction.
 * @return    - Direction to steer scaled to maxforce.
 *
 * @see "Steering Behaviors For Autonomous Characters",
 * http://www.red3d.com/cwr/steer/gdc99/
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
			// should give greator reaction
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
	pos        = sf::Vector2f(rnd() * windowx / 2.0f, rnd() * windowy / 2.0f);
	auto angle = (rnd() + 1.0f) * M_PI;
	vel        = sf::Vector2f(maxspeed * cos(angle), maxspeed * sin(angle));
	shape      = sf::CircleShape(20.0f, 3);
	shape.setPosition(pos - sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setFillColor(sf::Color::Yellow);
}

// Kan jeg exponere vektene og droppe denne funksjonen?
// Skal jeg impl jakt og fangst?
void Predator::update(float dt) {
	auto v1 = computeCohesion();

	vel += 2.0f * v1;
	vel = maxspeed * vel / length(vel);
	pos += vel * dt;

	// Wrap around the screen
	if (pos.x > windowx + shape.getRadius())
		pos.x -= windowx + shape.getRadius();
	else if (pos.x < -shape.getRadius())
		pos.x += windowx + shape.getRadius();

	if (pos.y > windowy + shape.getRadius())
		pos.y -= windowy + shape.getRadius();
	else if (pos.y < -shape.getRadius())
		pos.y += windowy + shape.getRadius();
}
// Main
// ***********************************************************************

int main() {
	// Init randomness
	// *******************************************************************

	// Generate seed
	std::string seed_str;
	std::cout << "Seed string: ";
	getline(std::cin, seed_str);
	std::seed_seq seed(seed_str.begin(), seed_str.end());

	// Initialize generator and distribution
	std::default_random_engine            generator(seed);
	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
	rnd = std::bind(distribution, generator);

	// Create window
	sf::RenderWindow window(sf::VideoMode(windowx, windowy), "Perlin noise");
	window.setFramerateLimit(30);

	// Setup boids
	gBoids.resize(num_obj);
	for (size_t i = 0; i < num_obj; i++)
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
