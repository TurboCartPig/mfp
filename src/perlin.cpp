#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <random>
#include <functional>

// Constants
// ***********************************************************************

const unsigned int windowx = 1000;
const unsigned int windowy = 1000;
const unsigned int SG_SIZE = 100;

// Globals
// ***********************************************************************
std::vector<sf::Vector2f> gSeedgrid;
std::function<float()> rnd;

// Helper functions
// ***********************************************************************

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

// float lerp
float lerp(float a, float b, float t) {
	return (1.0f - t) * a + t * b;
}

// vector floor
sf::Vector2f floor(sf::Vector2f a) {
	return sf::Vector2f(floor(a.x), floor(a.y));
}

// float fraction
float fract(float a) {
	return a - floor(a);
}

// vector fraction
sf::Vector2f fract(sf::Vector2f a) {
	return a - floor(a);
}

// vector dot pruduct
float dot(sf::Vector2f a, sf::Vector2f b) {
	return a.x * b.x + a.y * b.y;
}

// float clamp
float clamp(float a, float ma, float mi) {
	return fmax(fmin(a, mi), ma);
}

// Perlin impl from https://thebookofshaders.com
// ***********************************************************************

float random(sf::Vector2f st) {
    return fract(sin(dot(st,
                         sf::Vector2f(12.9898,78.233)))
							* 43758.5453123);
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
	auto e = lerp(d1, d2, v);

	return e;
}

// Perlin impl based on provided material
// ***********************************************************************

sf::Vector2f random2(sf::Vector2f p) {
	return gSeedgrid[p.x * SG_SIZE + p.y];
}

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

	// Smootstep interpolation
	auto u = gp.x * gp.x * (3.0f - 2.0f * gp.x);
	auto v = gp.y * gp.y * (3.0f - 2.0f * gp.y);

	// Bilinear interpolation
	auto d1 = lerp(c1, c2, u);
	auto d2 = lerp(c3, c4, u);
	auto e = lerp(d1, d2, v);

	e = (e + sqrt(2.0f) / 2.0f / sqrt(2.0f));

	// We need to clamp as we are going to get floating point
	// errors and extreme numbers
	return clamp(e, 0.0f, 1.0f);
}

// ***********************************************************************

// Assignment noise
float perlin3(sf::Vector2f p) {
	return (perlin2(p, 256)
		  + 0.5f  * perlin2(p, 128)
		  + 0.25f * perlin2(p, 64)) / 1.75f;
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

	// Generate seed
	std::string seed_str;
	std::cout << "Seed string: "; getline(std::cin, seed_str);
	std::seed_seq seed(seed_str.begin(), seed_str.end());

	// Initialize generator and distribution
	std::default_random_engine generator(seed);
	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
	rnd = std::bind(distribution, generator);

	// Setup seedgrid
	gSeedgrid = std::vector<sf::Vector2f>(SG_SIZE * SG_SIZE);
	for (size_t i = 0; i < SG_SIZE * SG_SIZE; i++) {
		auto r = (rnd() + 1.0f) * M_PI;
		gSeedgrid[i] = sf::Vector2f(cos(r), sin(r));
	}

	// Setup sfml stuff
	// *******************************************************************

	// Create window
	sf::RenderWindow window(
			sf::VideoMode(windowx, windowy),
			"Perlin noise"
		);
	window.setFramerateLimit(30);

	// Screenshot
	sf::Image capture;

	sf::Texture texture;
	// Create texture
	if (!texture.create(windowx, windowy)) {
		std::cout << "Error: Texture not created" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Create table of pixels
	// times 4 because pixels = (red, green, blue, alpha)
	uint8_t* pixels = new uint8_t[windowx * windowy * 4];

	// Generate an image from noise
	// *******************************************************************
	for (size_t x = 0; x < windowx; x++) {
		for (size_t y = 0; y < windowy; y++) {
			uint8_t e = perlin3(sf::Vector2f(x, y)) * 255;
			pixels[4*(x+y*windowx)  ] = e;
			pixels[4*(x+y*windowx)+1] = e;
			pixels[4*(x+y*windowx)+2] = e;
			pixels[4*(x+y*windowx)+3] = 255; // Should always be 255!!!
		}
	}

	texture.update(pixels);

	// Create a sprite and assign texture
	sf::Sprite sprite;
	sprite.setTexture(texture);

	float t = 0.0f;
	float inc = 1.0f;
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

				case sf::Event::KeyPressed:
					capture.create(windowx, windowy, pixels);
					capture.saveToFile("output.png");
					break;

				default:
					break;
			}
		}

		float dt = clock.restart().asSeconds();
		t += inc * dt;

		if (t > 3.0f || t < 0.0f)
			inc = -1.0f * inc;

		// Experiment with using time as a variable in noise
		/* // Generate an image from noise */
		/* // ******************************************************************* */
		/* for (size_t x = 0; x < windowx; x++) { */
		/* 	for (size_t y = 0; y < windowy; y++) { */
		/* 		uint8_t e = perlin3(sf::Vector2f(x, y), t) * 255; */
		/* 		pixels[4*(x+y*windowx)  ] = e; */
		/* 		pixels[4*(x+y*windowx)+1] = e / 2; */
		/* 		pixels[4*(x+y*windowx)+2] = e / 3; */
		/* 		pixels[4*(x+y*windowx)+3] = 255; // Should always be 255!!! */
		/* 	} */
		/* } */

		/* texture.update(pixels); */

		// Rendering
		window.clear();

		window.draw(sprite);

		window.display();
	}

	return EXIT_SUCCESS;
}
