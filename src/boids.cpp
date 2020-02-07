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
const unsigned int num_obj = 100;

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

	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::CircleShape shape;
};

// Helper functions
// ***********************************************************************

/**
 * Returns the lenght of the given vector.
 */
float length(sf::Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }

/**
 * Computes whether or not a boid is visible from another.
 *
 * @param a - The first boid.
 * @param b - The second boid.
 * @return  - Is the boid b visible from boid a?
 */
bool visible(const Boid *a, const Boid *b) {
	auto vel = a->getVelocity();
	vel /= length(a->getVelocity()); // Normalize

	auto diff = b->getPosition() - a->getPosition();
	auto dist = length(diff);
	diff /= dist;					 // Normalize

	auto angle_to_b = atan2(vel.y, vel.x) - atan2(diff.y, diff.x);

	/* auto ret = (dist < 100.0f */
	/* 			|| angle_to_b > M_PI * 2.0f */
	/* 			|| angle_to_b < -M_PI * 2.0f) */
	/* 		    && dist != 0.0f; */ 

	auto ret = dist < 100.0f && dist > 0.0f;

	return ret;
}

// Boid impl
// ***********************************************************************

Boid::Boid() {
	pos = sf::Vector2f(rnd() * windowx / 2.0f, rnd() * windowy / 2.0f);
	vel = sf::Vector2f(rnd() * 50.0f, rnd() * 50.0f);
	shape = sf::CircleShape(10.0f, 3);
	shape.setPosition(pos - sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setFillColor(sf::Color::Red);
}

void Boid::update(float dt) {
	auto v1 = computeCohesion();
	auto v2 = computeSeparation();
	auto v3 = computeAlignment();

	/* vel = 50.0f * vel / length(vel); */
	vel += 0.0f * v1 + 1.5f * v2 + 1.0f * v3;

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

void Boid::draw(sf::RenderWindow &w) {
	shape.setPosition(pos - sf::Vector2f(shape.getRadius(), shape.getRadius()));
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

sf::Vector2f Boid::computeCohesion() {
	auto avg_pos = sf::Vector2f(0.0f, 0.0f);
	size_t num_vis = 0;

	for (size_t i = 0; i < num_obj; i++) {
		if (visible(this, gBoids[i])) {
			auto other = gBoids[i]->getPosition();
			avg_pos += other / length(other);
			num_vis++;
		}
	}

	if (num_vis > 0)
		return (avg_pos / (float)num_vis - pos);
	else
		return sf::Vector2f(0.0f, 0.0f);
}

sf::Vector2f Boid::computeSeparation() {
	auto ret = sf::Vector2f(0.0f, 0.0f);
	size_t num_vis = 0;

	for (size_t i = 0; i < num_obj; i++) {
		auto dis = length(gBoids[i]->getPosition() - pos);
		if (visible(this, gBoids[i])) {
			auto diff = pos - gBoids[i]->getPosition();
			/* diff /= dis; */
			diff /= dis;

			ret += diff;
			num_vis++;
			std::cout << "Separating: \n"
					  << "diff: " << diff.x << ", " << diff.y
					  << " dis: " << dis
					  << " ret: " << ret.x << ", " << ret.y
					  << " count: " << num_vis
					  << "\n";
		}
	}

	if (num_vis > 0)
		return (ret / (float)num_vis);
	else
		return sf::Vector2f(0.0f, 0.0f);
}

sf::Vector2f Boid::computeAlignment() {
	auto ret = sf::Vector2f(0.0f, 0.0f);

	return ret;
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
