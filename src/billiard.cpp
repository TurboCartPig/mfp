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
static const uint32_t windowx     = 1000;
static const uint32_t windowy     = 1000;
static const float    BALL_RADIUS = 10.0f;
static const size_t   BALL_COUNT  = 20;

// Globals
// ***********************************************************************

std::vector<sf::CircleShape *> gBalls;
std::vector<sf::Vector2f>      gVels;
std::function<float()>         rnd;

// Declarations
// ***********************************************************************

class Ball {
  public:
    sf::CircleShape shape;
    sf::Vector2f pos;
    sf::Vector2f vel;
    float mass;
};

// Helper Functions
// ***********************************************************************

bool isColiding(sf::CircleShape &a, sf::CircleShape &b) {
	auto minD = a.getRadius() + b.getRadius();
	auto dis  = length(b.getPosition() - a.getPosition());

	return minD >= dis;
}

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

	gBalls.reserve(BALL_COUNT);
	gVels.reserve(BALL_COUNT);
	for (size_t i = 0; i < BALL_COUNT; i++) {
		auto ball = new sf::CircleShape(BALL_RADIUS, 10);
		ball->setFillColor(sf::Color::Red);
		ball->setPosition(windowx / 2, windowy / 2);

		float speed = 100.0f;
		gVels[i]    = speed * sf::Vector2f(rnd(), rnd());

		gBalls.push_back(ball);
	}

	// Distribute the balls around the screen without intersections
	for (size_t i = 0; i < gBalls.size(); i++) {
		auto ball      = gBalls[i];
		bool colliding = false;

		ball->setPosition((rnd() + 1) * windowx / 2, (rnd() + 1) * windowy / 2);
		for (auto other : gBalls) {
			if (ball == other)
				continue;
			if (isColiding(*ball, *other))
				colliding = true;
		}

		if (colliding)
			i--;
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

		// Integration
		for (size_t i = 0; i < BALL_COUNT; i++) {
			gBalls[i]->setPosition(gBalls[i]->getPosition() + gVels[i] * dt);
		}

		// Check for intersections
		for (size_t i = 0; i < BALL_COUNT; i++) {
            auto ball = gBalls[i];
            auto mass1 = 1;
            auto mass2 = 1;

		    // Collision with other balls
			for (size_t j = 0; j < BALL_COUNT; j++) {
				if (i == j)
					continue;
				if (isColiding(*ball, *gBalls[j])) {
					std::cout << "Boink!";
					// Perform collision
					auto u2 = 
                    auto v1 = (mass1 - mass2) / (mass1 + mass2) + (2 * mass2) / (mass1 * mass2) * u2;
				}
			}

			// Collision with walls
			auto pos  = ball->getPosition();
			auto rad  = ball->getRadius();
			if (pos.x >= windowx - rad * 2) {
				pos.x      = windowx - rad * 2;
				gVels[i].x = -gVels[i].x;
			} else if (pos.x <= 0.0f) {
				pos.x      = 0.0f;
				gVels[i].x = -gVels[i].x;
			}
			if (pos.y >= windowy - rad * 2) {
				pos.y      = windowy - rad * 2;
				gVels[i].y = -gVels[i].y;
			} else if (pos.y <= 0.0f) {
				pos.y      = 0.0f;
				gVels[i].y = -gVels[i].y;
			}
			ball->setPosition(pos);
		}

		// Rendering
		// ***************************************************************

		window.clear();

		for (auto ball : gBalls) {
			window.draw(*ball);
		}

		window.display();
	}

	return EXIT_SUCCESS;
}
