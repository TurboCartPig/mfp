/**
 * Live programming exercise where we implement tetris.
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

const float wSize    = 800;
const int   bSize    = 30;
const float tileSize = wSize / bSize;

const int       numColors        = 3;
const sf::Color color[numColors] = {sf::Color::Red, sf::Color::Blue,
                                    sf::Color::Green};

class Board;

typedef int Tile;

class Shape {
  public:
	static const int shSize   = 4;
	static const int numTiles = shSize * shSize;

	Tile         tile[numTiles]; // tile[i] == 0 means empty
	const float  timeStep = 0.5;
	float        t;
	sf::Vector2i pos;

	Shape() { init(); };

	Shape(const Shape &sh) {
		for (int i = 0; i < numTiles; i++)
			tile[i] = sh.tile[i];
		t   = sh.t;
		pos = sh.pos;
	};

	Shape &operator=(Shape &sh) {
		for (int i = 0; i < numTiles; i++)
			tile[i] = sh.tile[i];
		t   = sh.t;
		pos = sh.pos;
		return *this;
	};

	void update(float dt);
	void oneDown();
	void allDown();
	void rotateLeft();
	void rotateRight();
	void moveLeft();
	void moveRight();
	void init();

	void draw(sf::RenderTarget &w) {
		sf::CircleShape sh;
		sh.setRadius(tileSize / 2.0);
		for (int i = 0; i < numTiles; i++) {
			if (tile[i]) {
				sf::Vector2i ap(i % 4 + pos.x, i / 4 + pos.y);
				sh.setPosition(tileSize * sf::Vector2f(ap));
				sh.setFillColor(color[tile[i] % numColors]);
				w.draw(sh);
			}
		}
	}
};

class Board {
  public:
	const static int numTiles = bSize * bSize;
	Tile             tile[numTiles];

	Board() { init(); };

	void addShape(const Shape &sh);
	bool intersect(const Shape &sh);
	void init();
	void reduce();

	void draw(sf::RenderTarget &w) {
		sf::CircleShape sh;
		sh.setRadius(tileSize / 2.0);
		for (int i = 0; i < numTiles; i++) {
			if (tile[i]) {
				sf::Vector2i ap(i % bSize, i / bSize);
				sh.setPosition(tileSize * sf::Vector2f(ap));
				sh.setFillColor(color[tile[i] % numColors]);
				w.draw(sh);
			}
		}
	}
} board;

void Board::reduce() {}

void Board::addShape(const Shape &sh) {}

bool Board::intersect(const Shape &sh) { return false; }

void Board::init() {
	// Could be improved
	for (int i = 0; i < numTiles; i++)
		tile[i] = 0;
	for (int i = 0; i < bSize - 3; i++) {
		tile[i * bSize + 10] = 1;
		tile[i * bSize + 20] = 1;
	}
	for (int i = 10; i < 20; i++) {
		tile[(bSize - 4) * bSize + i] = 1;
	}
}

void Shape::oneDown() {
    pos.y += 1;
}

void Shape::allDown() {}

void Shape::rotateLeft() {}

void Shape::rotateRight() {}

void Shape::moveLeft() {
    pos.x -= 1;
}

void Shape::moveRight() {
    pos.x += 1;
}

void Shape::init() {
	t   = 0;
	pos = sf::Vector2i(bSize / 2 - 2, 0);
	// nonsense tile
	for (int i = 0; i < numTiles; i++)
		tile[i] = 0;
	tile[4] = 1;
	tile[5] = 1;
}

void Shape::update(float dt) {
    t += dt;
    if (t > timeStep) {
        pos.y += 1;
    }
}

int main() {
	sf::RenderWindow window(sf::VideoMode(wSize, wSize), "Tetris");

	sf::Clock clock;
	clock.restart();

	bool keyDown  = false;
	bool keyLeft  = false;
	bool keyRight = false;
	bool keyUp    = false;
	bool keyA     = false;
	bool keyD     = false;

	Shape sh;

	// run the program as long as the window is open
	while (window.isOpen()) {
		// check all the window's events that were triggered since the last
		// iteration of the loop
		sf::Event event;

		while (window.pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			if (!keyLeft)
				sh.moveLeft();
			keyLeft = true;
		} else
			keyLeft = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			if (!keyRight)
				sh.moveRight();
			keyRight = true;
		} else
			keyRight = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
			if (!keyDown)
				sh.oneDown();
			keyDown = true;
		} else
			keyDown = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
			if (!keyUp)
				sh.allDown();
			keyUp = true;
		} else
			keyUp = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			if (!keyA)
				sh.rotateLeft();
			keyA = true;
		} else
			keyA = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			if (!keyD)
				sh.rotateRight();
			keyD = true;
		} else
			keyD = false;

		float dt = clock.restart().asSeconds();

		sh.update(dt);
		board.reduce();

		window.clear(sf::Color::Black);

		sh.draw(window);
		board.draw(window);

		window.display();
	}

	return 0;
}
