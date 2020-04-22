/**
 * The solar system assignment.
 *
 * @author Dennis Kristiansen
 * @file solar.cpp
 */

#define _USE_MATH_DEFINES

#include "SFML/Graphics.hpp"

#include <cmath>

const int windowx = 1200;
const int windowy = 800;

class Planet {
  public:
	Planet(float dist, float s, float a, float r, float xy, sf::Color color,
	       sf::Vector2f c) {
		distanceFromCenter = dist;
		angle              = a;
		speed              = s;
		xyRatio            = xy;
		center             = c;
		offset.x = offset.y = r;
		shape.setFillColor(color);
		shape.setRadius(r);
		shape.setPosition(center);
	}

	void update(double dt) {
		velocity.x = xyRatio * cos(angle);
		velocity.y = (1.0F / xyRatio) * sin(angle);
		position   = velocity * distanceFromCenter + center;

		float angle_delta = atanf(speed / distanceFromCenter);
		angle += angle_delta * dt;

		shape.setPosition(position - offset);
	}

	sf::Vector2f getPosition() { return position; }

	void setCenter(sf::Vector2f c) { center = c; }

	void decDistance(double dt) {
		distanceFromCenter -= 15.0F * dt;
		if (distanceFromCenter <= shape.getRadius()) {
			visible = false;
		}
	}

	sf::CircleShape shape;
	bool            visible = true;

  private:
	float angle;
	float distanceFromCenter;
	float speed;
	float xyRatio;

	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f offset;
	sf::Vector2f center;
};

int main(int argc, char *argv[]) {
	// We open up a window
	sf::RenderWindow window(sf::VideoMode(windowx, windowy), "Solar");

	sf::Vector2f origo(windowx / 2.0F, windowy / 2.0F);

	// Init planets
	Planet sun(0.0F, 0.0F, 0.0F, 30.0F, 1.0F, sf::Color::Yellow, origo);
	Planet planeta(250.0F, 80.0F, 45.0F, 50.0F, 1.0F, sf::Color::Green, origo);
	Planet moon(150.0F, 200.0F, 0.0F, 20.0F, 1.0F, sf::Color::White,
	            planeta.getPosition());
	Planet planetb(350.0F, 100.0F, 30.0F, 50.0F, 1.3F, sf::Color::Cyan, origo);
	Planet lua(150.0F, 200.0F, 0.0F, 20.0F, 1.0F, sf::Color::White,
	           planetb.getPosition());
	Planet planets(400.0F, 50.0F, 90.0F, 45.0F, 1.0F, sf::Color::Red, origo);

	// We need to tell time. Restart the clock.
	sf::Clock clock;
	clock.restart();

	// This is our "Game Loop"
	while (window.isOpen()) {
		// We clear the event queue
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// We find the time since the last run of the loop
		float dt = clock.restart().asSeconds();

		// Clear buffer
		window.clear();

		//
		// Physics
		//
		planeta.update(dt);

		// Make the moon follow it's planet
		moon.setCenter(planeta.getPosition());
		moon.update(dt);

		planetb.update(dt);

		lua.setCenter(planetb.getPosition());
		lua.update(dt);

		planets.decDistance(dt);
		planets.update(dt);

		//
		// Rendering
		//
		window.draw(sun.shape);
		window.draw(planeta.shape);
		window.draw(moon.shape);
		window.draw(planetb.shape);
		window.draw(lua.shape);

		if (planets.visible)
			window.draw(planets.shape);

		// Display buffer
		window.display();
	}
}
