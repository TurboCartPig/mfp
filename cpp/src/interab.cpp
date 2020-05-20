/**
 * The interpolation assignment part a and b.
 *
 * See inter.cpp for part c.
 *
 *  @author Dennis Kristiansen
 *  @file interab.cpp
 */

#include "common.h"

const uint32_t WINDOWX = 1200;
const uint32_t WINDOWY = 800;
const uint32_t r       = 50;

uint8_t u8lerp(uint8_t a, uint8_t b, float t) {
	// t: [0, 1] |--> [0, 255]
	auto t_u8 = static_cast<uint8_t>(t * 255);

	return ((255 - t_u8) * a + t_u8 * b) / 255;
}

/**
 * Linear interpolation between two points.
 *
 * @param p_0 - The first point.
 * @param p_1 - The second point.
 * @param t   - t decides the degree of interpolation from p_0 to p_1.
 * @return    - The result of the interpolation.
 */
sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t) {
	return (1.0f - t) * a + t * b;
}

/**
 * Quadratic Bezier Curve.
 *
 * Interpolate between two points (p_0 and p_2) using
 * a control point (p_1).
 *
 * @param p_0 - The first control point.
 * @param p_1 - The second control point.
 * @param p_2 - The third control point.
 * @param t   - degree of interpolation along the curve.
 * @return    - The result of the interpolation.
 */
sf::Vector2f quadratic_bezier_curve(sf::Vector2f p_0, sf::Vector2f p_1,
                                    sf::Vector2f p_2, float t) {
	return (1.0f - t) * (1.0f - t) * p_0 + 2.0f * t * (1.0f - t) * p_1 +
	       t * t * p_2;
}

/**
 * Cubic Bezier Curve.
 *
 * @param p_n - The n-th control point.
 * @param t   - Degree of interpolation along curve.
 * @return    - The result of the interpolation.
 */
sf::Vector2f cubic_bezier_curve(sf::Vector2f p_0, sf::Vector2f p_1,
                                sf::Vector2f p_2, sf::Vector2f p_3, float t) {
	return static_cast<float>(pow((1.0f - t), 3)) * p_0 +
	       3.0f * t * static_cast<float>(pow((1.0f - t), 2)) * p_1 +
	       3.0f * static_cast<float>(pow(t, 2)) * (1.0f - t) * p_2 +
	       static_cast<float>(pow(t, 3)) * p_3;
}

/**
 * Smoothstep function.
 *
 * A generic smoothstep function that takes the curve (s) as a parameter.
 *
 * @param p_0 - The first point.
 * @param p_1 - The second point.
 * @param t   - The degree of interpolation along the curve.
 * @param s   - A curve defined as a function
 *				that takes t as a parameter and returns a new t.
 * @return    - The result of the interpolation.
 */
sf::Vector2f smoothstep(sf::Vector2f p_0, sf::Vector2f p_1, float t,
                        std::function<float(float)> s) {
	return (1.0f - s(t)) * p_0 + s(t) * p_1;
}

int main() {
	// Initialization
	// *******************************************************************
	sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY),
	                        "Interpolation Assignment - Part A and B");
	window.setFramerateLimit(30);

	sf::CircleShape obj_1, obj_2, obj_3, obj_4, disk;

	obj_1.setRadius(r);
	obj_1.setFillColor(sf::Color::Red);

	obj_2.setRadius(r);
	obj_2.setFillColor(sf::Color::Blue);

	obj_3.setRadius(r);
	obj_3.setFillColor(sf::Color::Green);

	obj_4.setRadius(r);
	obj_4.setFillColor(sf::Color::Yellow);

	disk.setRadius(r);
	disk.setFillColor(sf::Color::Black);
	disk.setPosition(sf::Vector2f(WINDOWX / 2 - r, WINDOWY / 2 - r));

	sf::Clock clock;
	clock.restart();

	// Individual time variables for some of the objects
	auto t_1 = 0.0f;
	auto t_2 = 0.0f;
	auto t_3 = 0.0f;

	// Game loop
	// *******************************************************************
	while (window.isOpen()) {
		// Event handling
		// ***************************************************************

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		auto dt = clock.restart().asSeconds();

		// Lerp object 1
		// ***************************************************************

		t_1 += dt;
		if (t_1 > 6.0f) {
			t_1 = 0.0f;
		}

		if (t_1 <= 3.0f) {
			obj_1.setPosition(lerp(sf::Vector2f(r, r),
			                       sf::Vector2f(WINDOWX - r, WINDOWY - r),
			                       t_1 / 3.0f));
		} else {
			obj_1.setPosition(lerp(sf::Vector2f(WINDOWX - r, WINDOWY - r),
			                       sf::Vector2f(r, r), (t_1 - 3.0f) / 3.0f));
		}

		// Interpolate object 2
		// ***************************************************************

		t_2 += dt;
		if (t_2 > 10.0f) {
			t_2 = 0.0f;
		}

		if (t_2 <= 5.0f) {
			obj_2.setPosition(quadratic_bezier_curve(
			    sf::Vector2f(r, r), sf::Vector2f(WINDOWX, 0.0f),
			    sf::Vector2f(WINDOWX - r, WINDOWY - r), t_2 / 5.0f));
		} else {
			obj_2.setPosition(quadratic_bezier_curve(
			    sf::Vector2f(WINDOWX - r, WINDOWY - r),
			    sf::Vector2f(WINDOWX, 0.0f), sf::Vector2f(r, r),
			    (t_2 - 5.0f) / 5.0f));
		}

		// Interpolate object 3
		// ***************************************************************

		t_3 += dt;
		if (t_3 > 12.0f) {
			t_3 = 0.0f;
		}

		auto order_3_curve = [](float t) { return t * t * (3.0f - 2.0f * t); };

		if (t_3 <= 6.0f) {
			obj_3.setPosition(smoothstep(sf::Vector2f(WINDOWX / 2, r),
			                             sf::Vector2f(WINDOWX / 2, WINDOWY - r),
			                             t_3 / 6.0f, order_3_curve));
		} else {
			obj_3.setPosition(smoothstep(sf::Vector2f(WINDOWX / 2, WINDOWY - r),
			                             sf::Vector2f(WINDOWX / 2, r),
			                             (t_3 - 6.0f) / 6.0f, order_3_curve));
		}

		// Interpolate object 4
		// ***************************************************************

		if (t_3 <= 6.0f) {
			auto u = pow(cos(M_PI / 2.0 * (t_3) / 6.0f), 2);
			obj_4.setPosition(cubic_bezier_curve(
			    sf::Vector2f(r, WINDOWY / 2), sf::Vector2f(WINDOWX / 3, 0),
			    sf::Vector2f(2 * WINDOWX / 3, WINDOWY),
			    sf::Vector2f(WINDOWX - r, WINDOWY / 2), u));
		} else {
			auto u = pow(cos(M_PI / 2.0 * (t_3 - 6.0f) / 6.0f), 2);
			obj_4.setPosition(cubic_bezier_curve(
			    sf::Vector2f(WINDOWX - r, WINDOWY / 2),
			    sf::Vector2f(2 * WINDOWX / 3, WINDOWY),
			    sf::Vector2f(WINDOWX / 3, 0), sf::Vector2f(r, WINDOWY / 2), u));
		}

		// Interpolate rgb values
		// ***************************************************************

		if (t_2 <= 2.5f) {
			disk.setFillColor(
			    sf::Color(u8lerp(0, 255, t_2 / 2.5f), 0.0f, 0.0f));
		} else if (t_2 <= 5.0f) {
			disk.setFillColor(
			    sf::Color(u8lerp(255, 0, (t_2 - 2.5f) / 2.5f), 0.0f, 0.0f));
		} else if (t_2 <= 7.5f) {
			disk.setFillColor(
			    sf::Color(0.0f, u8lerp(0, 255, (t_2 - 5.0f) / 2.5f), 0.0f));
		} else {
			disk.setFillColor(
			    sf::Color(0.0f, u8lerp(255, 0, (t_2 - 7.5f) / 2.5f), 0.0f));
		}

		// Drawing
		// ***************************************************************
		window.clear();

		window.draw(obj_1);
		window.draw(obj_2);
		window.draw(obj_3);
		window.draw(obj_4);
		window.draw(disk);

		window.display();
	}

	return EXIT_SUCCESS;
}
