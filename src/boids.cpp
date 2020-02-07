/**
 * A boids implemantation based on "Boids" (http://www.red3d.com/cwr/boids/) by Reynolds
 * and the course material.
 *
 * @author Dennis Kristiansen
 * @file boids.cpp
 * @date 1.2.2020
 */

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <functional>
#include <SFML/Graphics.hpp>

// Constants
// ***********************************************************************

const unsigned int windowx = 1000;
const unsigned int windowy = 1000;
const unsigned int num_obj = 50;

// Globals
// ***********************************************************************

std::vector<class Boid*> gBoids;
std::function<float()> rnd;

// Boid decl
// ***********************************************************************

class Boid {
public:
	Boid();
	void update(float dt);
	void draw(sf::RenderWindow &w);
	sf::Vector2f getPosition() const;
	sf::Vector2f getVelocity() const;

private:
	sf::Vector2f computeCohesion();
	sf::Vector2f computeSeparation();
	sf::Vector2f computeAlignment();

	sf::Vector2f steer(sf::Vector2f dir);

	// Max speed of the boid
	const float maxspeed = 50.0f;
	// Max force a rule can apply
	const float maxforce = 0.03;

	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::CircleShape shape;
};

// Helper functions
// ***********************************************************************

/**
 * Returns the lenght of the given vector.
 */
float length(const sf::Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }

sf::Vector2f clamp(const sf::Vector2f v, float len) {
	if (length(v) > len) {
		return len * v / length(v);
	} else
		return v;
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
	auto vel = a->getVelocity();
	auto a_angle = atan2(vel.y, vel.x);

	auto diff = b->getPosition() - a->getPosition();
	auto b_angle = atan2(diff.y, diff.x);

	auto angle = b_angle - a_angle;

	auto vis = angle < M_PI_2 && angle > -M_PI_2;

	/* std::cout << "Angle a: " << a_angle */
	/*  		  << " b: " << b_angle */
	/*  		  << " vis: " << vis */
	/*  		  << "\n"; */

	return vis;
}

// Boid impl
// ***********************************************************************

/**
 * Construct a boid
 *
 * Initializes the boid with a random position and velocity.
 */
Boid::Boid() {
	pos = sf::Vector2f(rnd() * windowx / 2.0f, rnd() * windowy / 2.0f);
	auto angle = (rnd() + 1.0f) * M_PI;
	vel = sf::Vector2f(maxspeed * cos(angle), maxspeed * sin(angle));
	shape = sf::CircleShape(10.0f, 3);
	shape.setPosition(
			pos - sf::Vector2f(shape.getRadius(), shape.getRadius()));
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

	vel += 0.6f * v1 + 1.5f * v2 + 0.5f * v3;
	if (length(vel) > maxspeed)
		vel /= length(vel) / maxspeed;

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
 */
void Boid::draw(sf::RenderWindow &w) {
	// Update position and rotation of the boid
	shape.setPosition(
			pos - sf::Vector2f(shape.getRadius(), shape.getRadius()));
	auto nvel = vel / length(vel);

	// 210 was found using experimentation
	shape.setRotation(atan2(nvel.y, nvel.x) / M_PI * 180.0f + 210.0f);

	w.draw(shape);
}

sf::Vector2f Boid::getPosition() const {
	return pos;
}

sf::Vector2f Boid::getVelocity() const {
	return vel;
}

/**
 * Reynolds steering function.
 * 
 * @param dir - Desired direction.
 * @return    - Direction to steer.
 *
 * @see "Steering Behaviors For Autonomous Characters",
 * http://www.red3d.com/cwr/steer/gdc99/
 */
sf::Vector2f Boid::steer(const sf::Vector2f dir) {
		auto ret = maxspeed * dir / length(dir);
		ret -= vel;
		ret = clamp(ret, maxforce);

		return ret;
}

/**
 * Compute the steering vector based on the cohesion rule.
 */
sf::Vector2f Boid::computeCohesion() {
	auto avg_pos = sf::Vector2f(0.0f, 0.0f);
	size_t num_vis = 0;

	for (auto other : gBoids) {
		if (visible(this, other)) {
		/* if (true) { */
			auto other_pos = other->getPosition();
			/* avg_pos += other_pos / length(other_pos); */
			avg_pos += other_pos;
			num_vis++;

			/* std::cout << "Cohering: \n" */
			/* 	      << "ret: " << avg_pos.x << ", " << avg_pos.y */
			/* 		  << " count: " << num_vis */
			/* 		  << "\n"; */
		}
	}

	// Get the average vector
	avg_pos /= (float)num_vis;

	// Avoid dividing by zero
	if (num_vis > 0) {
		return steer(avg_pos);
	} else
		return sf::Vector2f(0.0f, 0.0f);
}

/**
 * Compute the steering vector based on the separation rule.
 */
sf::Vector2f Boid::computeSeparation() {
	auto ret = sf::Vector2f(0.0f, 0.0f);
	size_t num_vis = 0;

	for (auto other : gBoids) {
		auto diff = pos - other->getPosition();
		auto dist = length(diff);
		if (visible(this, other) && dist > 0.0f && dist < 200.0f) {
			diff /= dist;
			ret += diff;
			num_vis++;

			/* std::cout << "Separating: \n" */
			/* 		  << "diff: " << diff.x << ", " << diff.y */
			/* 		  << " dis: " << dist */
			/* 		  << " ret: " << ret.x << ", " << ret.y */
			/* 		  << " count: " << num_vis */
			/* 		  << "\n"; */
		}
	}

	// Get the average vector
	ret /= (float)num_vis;

	if (num_vis > 0) {
		return steer(ret);
	} else
		return sf::Vector2f(0.0f, 0.0f);
}

/**
 * Compute the steering vector based on the alignment rule.
 */
sf::Vector2f Boid::computeAlignment() {
	auto ret = sf::Vector2f(0.0f, 0.0f);
	size_t num_vis = 0;
	
	for (auto other : gBoids) {
		auto dist = length(pos - other->getPosition());
		if (dist > 0.0f && dist < 150.0f) {
			ret += other->getVelocity();
			num_vis++;
		}
	}

	// Get the average vector
	ret /= (float)num_vis;

	if (num_vis > 0)
		return steer(ret);
	else
		return sf::Vector2f(0.0f, 0.0f);
}

// Main
// ***********************************************************************

int main() {
	// Init randomness
	// *******************************************************************

	// Generate seed
	std::string seed_str;
	std::cout << "Seed string: "; getline(std::cin, seed_str);
	std::seed_seq seed(seed_str.begin(), seed_str.end());

	// Initialize generator and distribution
	std::default_random_engine generator(seed);
	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
	rnd = std::bind(distribution, generator);

	// Create window
	sf::RenderWindow window(
			sf::VideoMode(windowx, windowy),
			"Perlin noise"
		);
	window.setFramerateLimit(30);

	// Setup boids
	gBoids.resize(num_obj);
	for (size_t i = 0; i < num_obj; i++) {
		gBoids[i] = new Boid();
	}

	sf::Clock clock;
	clock.restart();

	// Game loop
	// *******************************************************************
	while (window.isOpen()) {
		// Event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed: 
					window.close();
					break;

				default:
					break;
			}
		}

		float dt = clock.restart().asSeconds();

		// Update boids
		for (auto boid : gBoids) {
			boid->update(dt);
		}

		// Rendering
		window.clear();

		// Draw boids
		for (auto boid : gBoids) {
			boid->draw(window);
		}

		window.display();
	}

	// Cleanup
	for (auto boid : gBoids) {
		delete boid;
	}

	return EXIT_SUCCESS;
}
