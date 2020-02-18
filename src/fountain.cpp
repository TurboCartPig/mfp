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

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

// Constants
// ***********************************************************************
static const uint32_t windowx       = 1000;
static const uint32_t windowy       = 1000;
static const size_t   MAX_OBJ_COUNT = 1000;

// Globals
// ***********************************************************************

std::vector<class PhysicsObject *> gPhysicsObjects;
std::function<float()>             rnd;

// Class declarations
// ***********************************************************************

/**
 * A simple particle object.
 *
 * TODO: In order to turn this into a GPU particle, I need to:
 * 1. Make it immutable via using parametric integration
 * 2. Implement a separate particle emitter that can handle spawning and storing the particles
 * 3. Upload the particles to a buffer
 * 4. Use vertex shader to simulate physics and find current position of particle
 * 5. Use geometry shader to project particle to screen and generate a screen aligned billboard
 * 6. Use fragment shader to draw the particle
 *
 * @see https://www.genericgamedev.com/effects/parametric-particles/
 * @see https://www.genericgamedev.com/effects/parametric-gpu-accelerated-particles/
 */
class PhysicsObject {
  public:
	PhysicsObject();
	void         applyForce(sf::Vector2f force);
	void         update(float dt);
	void         draw(sf::RenderWindow &w);
	bool         isActive() const { return active; }
	sf::Vector2f getVelocity() const { return vel; }

  private:
	sf::Vector2f    pos;
	sf::Vector2f    vel;
	sf::Vector2f    acc;
	float           invMass;
	float           lifetime;
	float           maxLifetime;
	bool            active;
	sf::CircleShape shape;
};

// Helper function
// ***********************************************************************

/**
 * Map a value in a range to a value equally far between min and max in
 * different range.
 *
 * @tparam T        The type of the values in the ranges
 * @param  v        The value in the first range
 * @param  in_min   Lower limit of the first range
 * @param  in_max   Upper limit of the first range
 * @param  out_min  Lower limit of the second range
 * @param  out_max  Upper limit of the second range
 * @return          The value in the second range
 */
template <class T>
T map_range(T v, T in_min, T in_max, T out_min, T out_max) {
	return (v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * Returns the length of the given vector.
 */
float length(const sf::Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }

/**
 * Calculate the dot product of two vectors.
 *
 * @param a The first vector
 * @param b The second vector
 * @return  The dot product
 */
float dot(const sf::Vector2f a, const sf::Vector2f b) {
	return a.x * b.x + a.y * b.y;
}

/**
 * Project vector a onto vector b.
 *
 * @param a The vector being projected
 * @param b The vector being projected onto
 * @return  The projection vector
 */
sf::Vector2f project(const sf::Vector2f a, const sf::Vector2f b) {
	// auto aLen = length(a);
	// auto cosine = dot(a, b) / aLen * length(b);
	// return aLen * cosine * b;

	// auto d     = dot(a, b);
	// auto blen  = length(b);
	// auto anorm = a / length(a);
	// auto proj  = b * d / (blen * blen);
	// return proj;

	// Remember to surround the length in parenthesis
	// With: Divide vector
	// Without: Multiply with vector
	return b * dot(a, b) / (length(b) * length(b));
}

// Class implementation
// ***********************************************************************

PhysicsObject::PhysicsObject() {
	auto angle = map_range(rnd(), -1.0f, 1.0f, -135.0f, -45.0f);
	auto speed = 300.0f;

	// Can I give the object an impulse instead of setting the velocity
	pos = sf::Vector2f(windowx / 2, windowy);
	vel = sf::Vector2f(speed * cos(angle), speed * sin(angle));
	acc = sf::Vector2f(0.0f, 0.0f);

	invMass = 1.0f / 2.0f;

	lifetime    = 0.0f;
	maxLifetime = map_range(rnd(), -1.0f, 1.0f, 3.0f, 10.0f);
	// maxLifetime = 10.0f;

	active = true;

	shape = sf::CircleShape(10.0f, 10);
	shape.setFillColor(sf::Color(255, 185, 20, 255));
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
	auto outside = pos.x + d < 0.0f || pos.x > windowx + d ||
	               pos.y + d < 0.0f || pos.y > windowy + d;

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

	// Generate seed
	std::string seed_str;
	// std::cout << "Seed string: ";
	// getline(std::cin, seed_str);
	seed_str = "just so that I don't have to type it in every time";
	std::seed_seq seed(seed_str.begin(), seed_str.end());

	// Initialize generator and distribution
	std::default_random_engine            generator(seed);
	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
	rnd = std::bind(distribution, generator);

	// Create window
	sf::RenderWindow window(sf::VideoMode(windowx, windowy), "The Fountain");
	window.setFramerateLimit(30);

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
		for (size_t i = 0; i < 2; i++) {
			// Emplace_back takes the parameters of
			// the constructor and constructs the object internally.
			// This solves a lot of weird issues in C++
			// gPhysicsObjects.emplace_back();

			if (gPhysicsObjects.size() < MAX_OBJ_COUNT)
				gPhysicsObjects.push_back(new PhysicsObject);
		}

		// Apply forces
		for (auto o : gPhysicsObjects) {
			o->applyForce(gravity);

			// FIXME: This is weird
			// auto mywind = project(o->getVelocity(), wind);
			// auto mywind = project(wind, o->getVelocity());
			auto mywind = sf::Vector2f(wind.x - o->getVelocity().x, 0.0f);
			o->applyForce(mywind);
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
