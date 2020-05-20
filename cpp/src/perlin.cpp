/**
 * The perlin noise assignment.
 *
 * @author Dennis Kristiansen
 * @file perlin.cpp
 */

#include "common.h"

// Constants
// ***********************************************************************

constexpr uint32_t WINDOWX = 1000;
constexpr uint32_t WINDOWY = 1000;
constexpr uint32_t SG_SIZE = 100;

// Globals
// ***********************************************************************
std::vector<sf::Vector2f> gSeedgrid;
std::function<float()>    rnd;

// Helper functions
// ***********************************************************************

/**
 * Linear interpolation between two points.
 *
 * @param a - The first point.
 * @param a - The second point.
 * @param t - t decides the degree of interpolation from a to b.
 * @return  - The result of the interpolation.
 */
template <class T>
T lerp(T a, T b, float t) {
	return (1.0f - t) * a + t * b;
}

// vector floor
sf::Vector2f floor(sf::Vector2f a) {
	return sf::Vector2f(floor(a.x), floor(a.y));
}

// vector dot product
float dot(sf::Vector2f a, sf::Vector2f b) { return a.x * b.x + a.y * b.y; }

/**
 * Fraction. Get the fraction part of a type.
 *
 * @param a - Var to get fraction from
 * @return  - The fraction part of the input
 */
template <class T>
T fract(T a) {
	return a - floor(a);
}

/**
 * Clamp input to given range.
 *
 * @param a   - Param to clamp
 * @param min - Minimum bound of the range
 * @param max - Maximum bound of the range
 * @return    - The clamped value
 */
template <class T>
T clamp(T a, T min, T max) {
	return (a > max) ? max : (a < min) ? min : a;
}

// Perlin impl from https://thebookofshaders.com
// ***********************************************************************

float random(sf::Vector2f st) {
	return fract(sin(dot(st, sf::Vector2f(12.9898, 78.233))) * 43758.5453123);
}

float perlin(sf::Vector2f p, float tilesize) {
	// Scale so that the grid is visible
	p = p / tilesize;

	auto i = floor(p);
	auto f = fract(p);

	// Sample psudo random values for the four corners
	auto c1 = random(i);
	auto c2 = random(i + sf::Vector2f(1.0f, 0.0f));
	auto c3 = random(i + sf::Vector2f(0.0f, 1.0f));
	auto c4 = random(i + sf::Vector2f(1.0f, 1.0f));

	// Smootstep interpolation
	auto u = f.x * f.x * (3.0f - 2.0f * f.x);
	auto v = f.y * f.y * (3.0f - 2.0f * f.y);

	// Bilinear interpolation
	auto d1 = lerp(c1, c2, u);
	auto d2 = lerp(c3, c4, u);
	auto e  = lerp(d1, d2, v);

	return e;
}

// Perlin impl based on provided material
// ***********************************************************************

sf::Vector2f random2(sf::Vector2f p) { return gSeedgrid[p.x * SG_SIZE + p.y]; }

float perlin2(sf::Vector2f p, float tilesize) {
	p = p / tilesize;

	// Coord in the vertex grid
	auto ip = floor(p);

	// Coord inside the tile
	auto gp = fract(p);

	// The four corners of the tile
	auto w1 = gp;
	auto w2 = gp - sf::Vector2f(1.0f, 0.0f);
	auto w3 = gp - sf::Vector2f(0.0f, 1.0f);
	auto w4 = gp - sf::Vector2f(1.0f, 1.0f);

	auto v1 = random2(ip);
	auto v2 = random2(ip + sf::Vector2f(1.0f, 0.0f));
	auto v3 = random2(ip + sf::Vector2f(0.0f, 1.0f));
	auto v4 = random2(ip + sf::Vector2f(1.0f, 1.0f));

	auto c1 = dot(w1, v1);
	auto c2 = dot(w2, v2);
	auto c3 = dot(w3, v3);
	auto c4 = dot(w4, v4);

	// Smoothstep interpolation
	auto u = gp.x * gp.x * (3.0f - 2.0f * gp.x);
	auto v = gp.y * gp.y * (3.0f - 2.0f * gp.y);

	// Bilinear interpolation
	auto d1 = lerp(c1, c2, u);
	auto d2 = lerp(c3, c4, u);
	auto e  = lerp(d1, d2, v);

	e = (e + sqrt(2.0f) / 2.0f / sqrt(2.0f));

	// We need to clamp as we are going to get floating point
	// errors and extreme numbers
	return clamp(e, 0.0f, 1.0f);
}

// ***********************************************************************

// Assignment noise
float perlin3(sf::Vector2f p) {
	return (perlin2(p, 256) + 0.5f * perlin2(p, 128) + 0.25f * perlin2(p, 64)) /
	       1.75f;
}

/* float perlin3(sf::Vector2f p, float t) { */
/* 	p.y += t * 10.0f; */
/* 	p.x += t * 124.0f; */

/* 	return ( */
/* 		perlin2(p, 128) */
/* 	  + perlin2(p, 256)) */
/* 	  / 2.0f; */
/* } */

// ***********************************************************************

int main() {
	// Init randomness
	// *******************************************************************
	std::random_device                    rd;
	std::default_random_engine            generator(rd());
	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
	rnd = std::bind(distribution, generator);

	// Setup seedgrid
	gSeedgrid.resize(SG_SIZE * SG_SIZE);
	for (size_t i = 0; i < SG_SIZE * SG_SIZE; i++) {
		auto r       = (rnd() + 1.0f) * M_PI;
		gSeedgrid[i] = sf::Vector2f(cos(r), sin(r));
	}

	// Setup sfml stuff
	// *******************************************************************

	// Create window
	sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY), "Perlin noise");
	window.setFramerateLimit(30);

	// Screenshot
	sf::Image    capture;
	sf::Texture  capture_texture;
	sf::Vector2u windowSize = window.getSize();

	sf::Texture texture;
	// Create texture
	if (!texture.create(WINDOWX, WINDOWY)) {
		std::cout << "Error: Texture not created" << std::endl;
		exit(EXIT_FAILURE);
	}

	sf::Image dumb;
	if (!dumb.loadFromFile("../dumb.png")) {
		std::cout << "Error: Texture not created" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Create table of pixels
	// times 4 because pixels = (red, green, blue, alpha)
	auto *pixels = new uint8_t[WINDOWX * WINDOWY * 4];

	// Generate an image from noise
	// *******************************************************************
	for (size_t x = 0; x < WINDOWX; x++) {
		for (size_t y = 0; y < WINDOWY; y++) {
			auto    c                     = dumb.getPixel(x, y);
			uint8_t e                     = perlin3(sf::Vector2f(x, y)) * 255;
			pixels[4 * (x + y * WINDOWX)] = clamp(c.r + e, 0, 255);
			pixels[4 * (x + y * WINDOWX) + 1] = clamp(c.g + e, 0, 255);
			pixels[4 * (x + y * WINDOWX) + 2] = clamp(c.b + e, 0, 255);
			pixels[4 * (x + y * WINDOWX) + 3] = 255; // Should always be 255!!!
		}
	}

	texture.update(pixels);

	auto   center     = sf::Vector2f(WINDOWX / 2, WINDOWY / 2);
	auto   resolution = 260.0f;
	auto   intensity  = 30;
	auto   radius     = 550.0f;
	size_t c          = 0;
	auto   circle =
	    sf::VertexArray(sf::PrimitiveType::LineStrip, (size_t)resolution + 1);

	for (float i = 0; i <= 2.0 * M_PI; i += 2.0 * M_PI / resolution) {
		auto val =
		    perlin2(sf::Vector2f(cos(i) * intensity, sin(i) * intensity) +
		                sf::Vector2f(500, 500),
		            8);

		auto x = cos(i) * radius * val;
		auto y = sin(i) * radius * val;

		circle[c++].position = sf::Vector2f(x, y) + center;
	}

	// Assign the last point to the position of the first
	circle[c].position = circle[0].position;

	// Create a sprite and assign texture
	sf::Sprite sprite;
	sprite.setTexture(texture);

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

		// Rendering
		window.clear();

		/* window.draw(sprite); */
		window.draw(circle);

		window.display();
	}

	return EXIT_SUCCESS;
}
