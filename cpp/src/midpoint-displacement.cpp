/**
 * An implementation of the midpoint-displacement algorithm.
 *
 *  @author Dennis Kristiansen
 *  @file midpoint-displacement.cpp
 */

#include "common.h"

constexpr uint32_t WINDOWX = 1200;
constexpr uint32_t WINDOWY = 800;

int main() {
	// Init randomness
	// *******************************************************************
	std::random_device                    rd;
	std::default_random_engine            generator(rd());
	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
	auto rnd = std::bind(distribution, generator);

	size_t             n   = 10;            // The power of 2
	size_t             len = pow(2, n) + 1; // Number of points / length of v
	std::vector<float> v(len);              // The data
	size_t             step   = len - 1;    // Initial step value
	float displacement_factor = 0.5f;       // Initial displacement factor.
	                                        // (How much the points vary)

	// Initialize both ends
	v.front() = rnd();
	v.back()  = rnd();

	// Midpoint displacement
	for (size_t i = 0; i < n; i++) {                 // Subdivides
		for (size_t j = 0; j < len - 1; j += step) { // First sample
			auto k   = j + step;                     // Second sample
			auto mid = (j + k) / 2;                  // Midpoint
			v[mid]   = 0.5f * (v[j] + v[k]) + displacement_factor * rnd();

			std::cout << "Coord: (" << std::setw(2) << j << ", " << std::setw(2)
			          << k << ") Mid: " << std::setw(2) << mid << std::endl;
		}

		step /= 2;
		displacement_factor /= 2;
	}

	// Setup pretty printing
	std::cout << "\n" << std::setprecision(3) << std::fixed << std::showpoint;

	// Print values
	for (auto val : v) {
		std::cout << "Value: " << std::setw(4) << val << std::endl;
	}

	// Create window
	sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY),
	                        "Mid-point displacement Algorithm");
	window.setFramerateLimit(30);

	// Use a line to represent the result
	sf::VertexArray shape(sf::PrimitiveType::LineStrip, len);
	for (size_t i = 0; i < len; i++) {
		shape[i].position =
		    sf::Vector2f((float)i / (float)(len - 1) * (float)WINDOWX,
		                 (v[i] + 1.25f) * WINDOWY / 2);
	}

	// Game loop
	// ****************************************************************
	while (window.isOpen()) {
		// Event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Rendering
		window.clear();

		window.draw(shape);

		window.display();
	}

	return EXIT_SUCCESS;
}
