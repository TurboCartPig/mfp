/**
 * The interpolation assignment part c.
 *
 * See interab.cpp for part a and b.
 *
 *  @author Dennis Kristiansen
 *  @file inter.cpp
 */

#include "common.h"

// Constants
// ***********************************************************************
const float        r       = 160.0f; // Radius of the circle
const unsigned int num     = 100;    // Number of triangles
const unsigned int WINDOWX = 1200;
const unsigned int WINDOWY = 800;
const sf::Vector2f center(WINDOWX / 2.0f,
                          WINDOWY / 2.0f // Coord of the center point
);

// Forward declarations
// ***********************************************************************
sf::Vector2f square(float i);
sf::Vector2f circle(float i);

// Functions
// ***********************************************************************

/*
 * Point on square from angle.
 *
 * @param i - [0, num] Function computes angle from this.
 * @return  - Point on the square.
 */
sf::Vector2f square(float i) {
	sf::Vector2f ret;
	auto         angle = 2.0f * M_PI * i / num;

	if (angle <= M_PI / 4.0) {
		ret.x = r;
		ret.y = r * sin(angle) * 2.0 / sqrt(2.0);
	} else if (angle <= M_PI / 2.0) {
		ret.x = r * cos(angle) * 2.0 / sqrt(2.0);
		ret.y = r;
	} else if (angle <= 3.0 * M_PI / 4.0) {
		ret.x = r * cos(angle) * 2.0 / sqrt(2.0);
		ret.y = r;
	} else if (angle <= M_PI) {
		ret.x = -r;
		ret.y = r * sin(angle) * 2.0 / sqrt(2.0);
	} else if (angle <= 5 * M_PI / 4.0) {
		ret.x = -r;
		ret.y = r * sin(angle) * 2.0 / sqrt(2.0);
	} else if (angle <= 3.0 * M_PI / 2.0) {
		ret.x = r * cos(angle) * 2.0 / sqrt(2.0);
		ret.y = -r;
	} else if (angle <= 7.0 * M_PI / 4.0) {
		ret.x = r * cos(angle) * 2.0 / sqrt(2.0);
		ret.y = -r;
	} else {
		ret.x = r;
		ret.y = r * sin(angle) * 2.0 / sqrt(2.0);
	}

	/* std::cout << "Angle: " << angle << std::endl; */
	/* std::cout << "Coord: " << ret.x << ", " << ret.y << std::endl; */

	return ret + center;
}

/**
 * Point on a cricle from angle.
 *
 * @param i - [0, num] Function computes angle from this.
 * @return  - Point on the circle.
 */
sf::Vector2f circle(float i) {
	return sf::Vector2f(r * cos(2.0 * M_PI * i / num),
	                    r * sin(2.0 * M_PI * i / num)) +
	       center;
}

int main() {
	sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY),
	                        "Interpolation Assignment");
	window.setFramerateLimit(30);

	// Triangle
	// ******************************************************************
	sf::ConvexShape triangle;
	triangle.setPosition(sf::Vector2f(200.0f, 120.0f));
	triangle.setPointCount(3);
	triangle.setPoint(0, sf::Vector2f(0.0f, 0.0f));
	triangle.setPoint(1, sf::Vector2f(150.0f, 10.0f));
	triangle.setPoint(2, sf::Vector2f(120.0f, 90.0f));

	// Shape init
	// *****************************************************************

	sf::ConvexShape shape;
	shape.setFillColor(sf::Color::Red);
	shape.setPointCount(3 * num);

	auto      t   = 0.0f; // Time, used for interpolation
	auto      inc = 1;    // Are we incrementing or decrementing the t variable
	sf::Clock clock;
	clock.restart();

	// Game loop
	// ****************************************************************
	while (window.isOpen()) {
		// Event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		auto dt = clock.restart().asSeconds();

		// Should we switch between incrementing or decrementing
		if (t >= 1.0f || t <= 0.0f)
			inc = -inc;

		t += inc * dt;

		// Interpolation and triangle generation
		for (std::size_t i = 0, c = 0; i < num; i++) {
			shape.setPoint(c++, center);
			shape.setPoint(c++, lerp(square(i), circle(i), t / 1.0));
			shape.setPoint(c++, lerp(square(i + 1), circle(i + 1), t / 1.0));
		}

		// Rendering
		window.clear();

		window.draw(triangle);
		window.draw(shape);

		window.display();
	}

	return EXIT_SUCCESS;
}
