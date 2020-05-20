/**
 * The fountain exercise.
 *
 * a) The particles experience gravity only.
 * b) The particles experience gravity and wind from the right.
 * c) Choose a color scheme so that the particles fade from bright orange to
 * black. d) Save a screenshot of c to disk. e) Improve the particle system. 5%
 * of particles explode at random short time after spawning and spawn 10 new
 * particles traveling outward from the explosion. f) Save a screenshot of e to
 * disk.
 *
 * @author Dennis Kristiansen
 * @file fountain.cpp
 */

#include "common.h"
#include "math.h"

// Constants
// ***********************************************************************
constexpr uint32_t WINDOWX                      = 1000;
constexpr uint32_t WINDOWY                      = 1000;
constexpr size_t   MAX_OBJ_COUNT                = 100000;
constexpr float    PARTICLE_RADIUS              = 1.0f;
constexpr float    PARTICLE_EXPLOSION_TIME      = 1.0f;
constexpr size_t   PARTICLE_SPAWN_RATE          = 50;
constexpr size_t   PARTICLE_EXPLOSION_PARTICLES = 10;

// Globals
// ***********************************************************************

std::vector<class PhysicsObject *> gPhysicsObjects;
std::function<float()>             rnd;

// Class declarations
// ***********************************************************************

/**
 * A simple particle object.
 */
class PhysicsObject {
  public:
	PhysicsObject();
	PhysicsObject(sf::Vector2f p, sf::Vector2f v);
	void         applyForce(sf::Vector2f force);
	void         update(float dt);
	void         draw(sf::RenderWindow &w);
	void         deactivate() { active = false; }
	bool         isActive() const { return active; }
	bool         shouldExplode() const { return explode; }
	sf::Vector2f getPosition() const { return pos; }
	sf::Vector2f getVelocity() const { return vel; }
	float        getLifetime() const { return lifetime; }

  private:
	sf::Vector2f    pos;
	sf::Vector2f    vel;
	sf::Vector2f    acc;
	float           invMass;
	float           lifetime;
	float           maxLifetime;
	bool            active;
	bool            explode;
	sf::CircleShape shape;
};

// Class implementation
// ***********************************************************************

PhysicsObject::PhysicsObject() {
	auto angle = map_range(rnd(), -1.0f, 1.0f, -135.0f, -45.0f);
	auto speed = map_range(rnd(), -1.0f, 1.0f, 100.0f, 500.0f);

	// Can I give the object an impulse instead of setting the velocity
	pos = sf::Vector2f(WINDOWX / 2, WINDOWY);
	vel = sf::Vector2f(speed * cos(angle), speed * sin(angle));
	acc = sf::Vector2f(0.0f, 0.0f);

	invMass = 1.0f / 2.0f;

	lifetime    = 0.0f;
	maxLifetime = map_range(rnd(), -1.0f, 1.0f, 3.0f, 10.0f);

	active = true;

	auto r  = map_range(rnd(), -1.0f, 1.0f, 0.0f, 100.0f);
	explode = r < 5.0f;

	shape = sf::CircleShape(PARTICLE_RADIUS, 8);
	shape.setFillColor(sf::Color(255, 185, 20, 255));
}

PhysicsObject::PhysicsObject(sf::Vector2f p, sf::Vector2f v) : PhysicsObject() {
	pos     = p;
	vel     = v;
	explode = false;
}

void PhysicsObject::applyForce(sf::Vector2f force) { acc += force * invMass; }

void PhysicsObject::update(float dt) {

	// Euler forward integration method?
	// Are there any issues with this integration method?
	pos += vel * dt;
	vel += acc * dt;
	acc *= 0.0f; // Reset acceleration to avoid accumulation

	// Is the particle outside the screen
	auto d       = shape.getRadius() * shape.getRadius();
	auto outside = pos.x + d < 0.0f || pos.x > WINDOWX + d ||
	               pos.y + d < 0.0f || pos.y > WINDOWY + d;

	lifetime += dt;

	// If the particle has reached the end of it's life or gone of the screen,
	// then we should stop drawing it
	if (lifetime > maxLifetime || outside) {
		active   = false;
		lifetime = maxLifetime; // Lifetime can become negative which messes
		                        // with the alpha
	}
}

void PhysicsObject::draw(sf::RenderWindow &w) {
	shape.setPosition(pos);

	// Transition from fully opaque to transparent over the lifetime of the
	// particle
	auto color = shape.getFillColor();
	color.a    = map_range(lifetime, 0.0f, maxLifetime, 255.0f, 0.0f);
	shape.setFillColor(color);

	w.draw(shape);
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

	// Init sfml stuff
	// *******************************************************************

	// Create window
	sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY), "The Fountain");
	window.setFramerateLimit(30);

	// Screenshot
	sf::Image    capture;
	sf::Texture  capture_texture;
	sf::Vector2u windowSize = window.getSize();

	// Init simulation stuff
	// *******************************************************************

	gPhysicsObjects.reserve(MAX_OBJ_COUNT);

	sf::Vector2f gravity(0.0f, 98.1f);
	sf::Vector2f wind(-190.0f, 0.0f);
	float        time = 0.0f;

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

				case sf::Event::KeyPressed:
					capture_texture.create(windowSize.x, windowSize.y);
					capture_texture.update(window);
					capture = capture_texture.copyToImage();
					capture.saveToFile("output.png");
					break;

				default: break;
			}
		}

		float dt = clock.restart().asSeconds();
		time += dt;

		// Delete inactive objects
		// We have to iterate over the array backwards
		// in order to avoid skipping an element when deleting
		for (size_t i = gPhysicsObjects.size(); i-- > 0;) {
			if (!gPhysicsObjects[i]->isActive()) {
				std::swap(gPhysicsObjects[i], gPhysicsObjects.back());
				delete gPhysicsObjects.back();
				gPhysicsObjects.pop_back();
			}
		}

		// Spawn new objects
		// FIXME:
		if (gPhysicsObjects.size() < MAX_OBJ_COUNT) {
			// Spawn new ones
			for (size_t i = 0; i < PARTICLE_SPAWN_RATE; i++) {
				// Emplace_back takes the parameters of
				// the constructor and constructs the object internally.
				// This solves a lot of weird issues in C++
				// gPhysicsObjects.emplace_back();

				gPhysicsObjects.push_back(new PhysicsObject);
			}

			// Spawn explosions
			for (size_t j = 0; j < gPhysicsObjects.size(); j++) {
				auto o = gPhysicsObjects[j];
				if (o->getLifetime() > PARTICLE_EXPLOSION_TIME &&
				    o->shouldExplode()) {
					std::cout << "Boom!";
					o->deactivate();

					// Spawn explosion particles
					for (size_t i = 0; i < PARTICLE_EXPLOSION_PARTICLES; i++) {
						auto pos = o->getPosition();
						auto angle =
						    map_range(rnd(), -1.0f, 1.0f, 0.0f, 360.0f);
						auto speed = 100.0f;
						auto vel   = sf::Vector2f(speed * cos(angle),
                                                speed * sin(angle));
						gPhysicsObjects.push_back(new PhysicsObject(pos, vel));
					}
				}
			}
		}

		// Apply forces
		for (auto o : gPhysicsObjects) {
			o->applyForce(gravity);

			// Apply wind force
			auto proj_w_V = project(o->getVelocity(), wind);
			o->applyForce(wind - proj_w_V);
		}

		// Update all physics objects
		for (auto o : gPhysicsObjects)
			o->update(dt);

		// Rendering
		// ***************************************************************

		window.clear();

		// Draw all the physics objects
		for (auto o : gPhysicsObjects)
			o->draw(window);

		window.display();
	}

	return EXIT_SUCCESS;
}
