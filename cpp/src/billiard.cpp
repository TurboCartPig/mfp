//
// Created by dennis on 2/25/20.
//

#include "math.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

// Constants
// ***********************************************************************
static const uint32_t windowx          = 1000;
static const uint32_t windowy          = 1000;
static const float    BALL_RADIUS      = 10.0f;
static const uint32_t BALL_POINT_COUNT = 10;
static const size_t   BALL_COUNT       = 20;

// Globals
// ***********************************************************************

// std::vector<sf::CircleShape *> gBalls;
// std::vector<sf::Vector2f>      gVels;
std::vector<class Ball> gBalls;
std::function<float()>  rnd;

// Declarations
// ***********************************************************************

/**
 * Axis Aligned Bounding Box.
 */
class AABB {
  public:
	AABB(sf::Vector2f min, sf::Vector2f max) {
		this->min = min;
		this->max = max;
	}

	//  private:
	sf::Vector2f min;
	sf::Vector2f max;
};

/**
 * Billiard ball.
 */
class Ball {
  public:
	/// Construct a new ball with a given position
	Ball(const sf::Vector2f pos, const sf::Vector2f vel) {
		this->shape = sf::CircleShape(BALL_RADIUS, BALL_POINT_COUNT);
		shape.setFillColor(sf::Color::Red);
		this->pos  = pos;
		this->vel  = vel;
		this->mass = 1;
	}

	/// Update the internal state of the ball
	void update(const float dt) {
		// Integration
		pos = pos + vel * dt;

		shape.setPosition(pos);
	}

	/// Draw the ball
	void draw(sf::RenderWindow &w) { w.draw(this->shape); }

	/// Is this ball intersecting another ball?
	bool isIntersecting(const Ball &other) {
		auto minD = this->shape.getRadius() + other.shape.getRadius();
		auto dis  = length(this->pos - other.pos);

		return minD >= dis;
	}

	/// Is this ball intersecting another AABB?
	// FIXME: Do I even need this?
	//	bool isIntersecting(const AABB &other) {
	//		auto rad = shape.getRadius();
	//
	//		return false;
	//	}

	/// Perform collision between Ball and AABB if intersecting
	void collide(const AABB &other) {
		auto rad = shape.getRadius();

		if (pos.x >= other.max.x - rad * 2) {
			pos.x = other.max.x - rad * 2;
			vel.x = -vel.x;
		} else if (pos.x <= other.min.x) {
			pos.x = other.min.x;
			vel.x = -vel.x;
		}

		if (pos.y >= other.max.y - rad * 2) {
			pos.y = other.max.y - rad * 2;
			vel.y = -vel.y;
		} else if (pos.y <= other.min.y) {
			pos.y = other.min.y;
			vel.y = -vel.y;
		}

		shape.setPosition(pos);
	}

	/// Perform collision between two balls if intersecting
	void collide(const Ball &other) {
		if (!isIntersecting(other))
			return;

		std::cout << "Boink!\n";

		// TODO: Now perform elastic collision
	}

	void setPosition(const sf::Vector2f pos) {
		this->pos = pos;
		shape.setPosition(pos);
	}

  private:
	sf::CircleShape shape;
	sf::Vector2f    pos;
	sf::Vector2f    vel;
	float           mass;
};

/**
 * Main entry point.
 * @return EXIT_SUCCESS
 */
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

	// Init sfml stuff
	// *******************************************************************

	// Create window
	sf::RenderWindow window(sf::VideoMode(windowx, windowy), "Billiard Balls");
	window.setFramerateLimit(30);

	// Screenshot
	sf::Image    capture;
	sf::Texture  capture_texture;
	sf::Vector2u windowSize = window.getSize();

	// Init simulation stuff
	// *******************************************************************

	// Create screen bounding box
	auto screen =
	    AABB(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(windowx, windowy));

	// Spawn all the balls
	gBalls.reserve(BALL_COUNT);
	for (size_t i = 0; i < BALL_COUNT; i++) {
		auto pos = sf::Vector2f(windowx / 2, windowy / 2);

		float speed = 100.0f;
		auto  vel   = speed * sf::Vector2f(rnd(), rnd());

		auto ball = Ball(pos, vel);
		gBalls.push_back(ball);
	}

	// Distribute the balls around the screen without intersections
	for (size_t i = 0; i < gBalls.size(); i++) {
		bool colliding;

		do {
			colliding = false;
			gBalls[i].setPosition(sf::Vector2f((rnd() + 1) * windowx / 2,
			                                   (rnd() + 1) * windowy / 2));
			for (size_t j = 0; j < gBalls.size(); j++) {
				if (i == j)
					continue;

				colliding = gBalls[i].isIntersecting(gBalls[j]);
			}
		} while (colliding);
	}

	float     time = 0.0f;
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

		// Simulation
		// ***************************************************************

		// Collisions
		for (auto &ball : gBalls) {
			// Collide with wall
			ball.collide(screen);

			// Collide with other balls
			for (const auto &other : gBalls) {
				// Skip if self
				if (&ball == &other)
					continue;

                ball.collide(other);
			}
		}

		// Normal update
		for (auto &ball : gBalls) {
			ball.update(dt);
		}

		// Rendering
		// ***************************************************************

		window.clear();

		for (auto &ball : gBalls)
			ball.draw(window);

		window.display();
	}

	return EXIT_SUCCESS;
}
