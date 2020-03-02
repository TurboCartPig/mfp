/**
 * GPU based particle system.
 *
 * Uses parametric integration in order to make the code simpler.
 * This has the limitation that we always calculate current position based
 * on initial position and time passed. So it requires the acceleration to be
 * constant. Which means that we can not dynamically add forces and have
 * particles be effected by them.
 *
 * TODO: Write a system based on transform feedback
 * @see [Particle System using Transform Feedback](http://ogldev.atspace.co.uk/www/tutorial28/tutorial28.html)
 *
 * @author Dennis Kristiansen
 * @file fountain-gpu.cpp
 */

#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>

// Constants
// ***********************************************************************
static const uint32_t windowx       = 1000;
static const uint32_t windowy       = 1000;
static const size_t   MAX_OBJ_COUNT = 1000;

// Globals
// ***********************************************************************

std::function<float()> rnd;

// Class declarations
// ***********************************************************************

/**
 * A shader program abstraction.
 *
 * Handles loading, compiling and linking shaders into a program.
 */
class ShaderProgram {
  public:
	explicit ShaderProgram(std::vector<std::string> &paths);
	~ShaderProgram();
	void   use();
	GLuint getProgram() const { return program; }

  private:
	GLuint program;
};

/**
 * Construct a shader program from multiple shader stages.
 *
 * The shaders stage is determent based on the file extension.
 * Eg. .vert for vertex shader and .geom for geometry shader.
 *
 * @note Does not handle being passed multiple shaders for the same stage.
 * @note Does not handle tessellation or compute shaders
 *
 * @param paths An array of paths to the shader source code.
 */
ShaderProgram::ShaderProgram(std::vector<std::string> &paths) {
	// Load all shaders from disk and compile them
	std::vector<GLuint> shaders(paths.size());
	for (size_t i = 0; i < paths.size(); i++) {
		auto path = paths[i];

		// Open the file for reading
		std::ifstream file(path, std::ifstream::in);
		if (file.bad() || !file.is_open()) {
			std::cout << "Error: Failed to open file\n";
			exit(EXIT_FAILURE);
		}

		// Read in the file line by line
		std::string source;
		std::string line;
		while (getline(file, line)) {
			source.append(line + "\n");
		}
		// Why can I not do this?
		// file >> source;
		file.close();

		// Find the shader type
		GLenum type;
		if (path.find(".vert") != std::string::npos)
			type = GL_VERTEX_SHADER;
		else if (path.find(".geom") != std::string::npos)
			type = GL_GEOMETRY_SHADER;
		else if (path.find(".frag") != std::string::npos)
			type = GL_FRAGMENT_SHADER;
		else {
			std::cout << "Error: Shader with unknown file extension: " << path
			          << std::endl;
			exit(EXIT_FAILURE);
		}

		// Compile the shader
		const char *const cstring = source.c_str();
		GLuint            shader  = glCreateShader(type);
		glShaderSource(shader, 1, &cstring, nullptr);
		glCompileShader(shader);

		// Get the compile status
		GLint success;
		char  log[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		glGetShaderInfoLog(shader, 512, nullptr, log);

		std::cout << "Compiling shader: " << path << std::endl;
		if (success == GL_FALSE) {
			std::cout << "\tShader type: " << type << std::endl;
			std::cout << "\tSource code:\n" << source << std::endl;
			std::cout << "\tCompilation status: " << (bool)success << std::endl;
			std::cout << "\tCompilation log:\n" << log << std::endl;
		}

		shaders[i] = shader;
	}

	// Build and link a shader program
	program = glCreateProgram();
	for (auto shader : shaders)
		glAttachShader(program, shader);

	glLinkProgram(program);

	// Get the link status
	GLint success;
	char  log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(program, 512, nullptr, log);

	std::cout << "Linking Program: " << std::endl;
	if (success == GL_FALSE) {
		std::cout << "\tLinking status: " << (bool)success << std::endl;
		std::cout << "\tLinking log:\n" << log << std::endl;
	}

	for (auto shader : shaders)
		glDeleteShader(shader);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(program); }

/**
 * Use this shader program for future graphics processing.
 */
void ShaderProgram::use() { glUseProgram(program); }

/**
 * A simple particle.
 */
struct Particle {
	sf::Vector2f pos; ///< The initial position of the particle
	sf::Vector2f vel; ///< The initial velocity of the particle
};

/**
 * Represents a group of particles.
 */
struct ParticleGroup {
	size_t       count;    ///< Number of particles in the group
	GLuint       vao;      ///< Vertex array object
	GLuint       vbo;      ///< Vertex buffer object
	sf::Vector2f acc;      ///< Constant acceleration to apply to particles
	float        lifetime; ///< How long has the particle group been alive

	/**
	 * Construct a particle group of size c.
	 *
	 * @param c Number of particles in the group
	 */
	ParticleGroup(const size_t c, const sf::Vector2f a) {
		count = c;
		vao = vbo = 0;
		lifetime  = 0.0f;
		acc       = a;
	}
};

/**
 * A simple particle emitter.
 */
class ParticleEmitter {
  public:
	explicit ParticleEmitter(GLuint program);
	~ParticleEmitter();
	void draw(float dt);
	void emit(size_t count, sf::Vector2f pos, sf::Vector2f vel,
	          sf::Vector2f acc, float velDiviation);

  private:
	std::vector<ParticleGroup *> particleGroups;
	sf::Clock                    clock;
	GLint                        timeLocation;
	GLint                        accLocation;
	GLuint                       shaderProgram;
};

ParticleEmitter::ParticleEmitter(const GLuint program) {
	clock = sf::Clock();
	clock.restart();

	// Get the location of shader uniforms for later use
	timeLocation  = glGetUniformLocation(program, "time");
	accLocation   = glGetUniformLocation(program, "acceleration");
	shaderProgram = program;
}

ParticleEmitter::~ParticleEmitter() {
	for (auto pg : particleGroups) {
		glDeleteBuffers(1, &pg->vbo);
		glDeleteBuffers(1, &pg->vao);
		delete pg;
	}
}

/**
 * Draw the particles
 */
void ParticleEmitter::draw(float dt) {

	// Delete dead particle groups
	for (size_t i = particleGroups.size(); i-- > 0;) {
		if (particleGroups[i]->lifetime > 2.0f) {
			std::swap(particleGroups[i], particleGroups.back());

			// Cleanup the particle group
			auto p = particleGroups.back();
			glDeleteBuffers(1, &p->vbo);
			glDeleteBuffers(1, &p->vao);
			delete p;

			particleGroups.pop_back();
		}
	}

	// Draw all the particles
	for (auto pg : particleGroups) {
		pg->lifetime += dt;

		glBindVertexArray(pg->vao);
		glProgramUniform1f(shaderProgram, timeLocation, pg->lifetime);
		glProgramUniform2f(shaderProgram, accLocation, pg->acc.x, pg->acc.y);
		glDrawArrays(GL_POINTS, 0, pg->count);
		glBindVertexArray(0);
	}
}

/**
 * Generate and emit particles based on parameters.
 *
 * @param count        How many particles to emit
 * @param pos          Initial position of all particles
 * @param vel          Initial velocity of all particles
 * @param velDiviation Max diviation from vel in degrees
 */
void ParticleEmitter::emit(size_t count, sf::Vector2f pos, sf::Vector2f vel,
                           sf::Vector2f acc, float velDiviation) {
	std::cout << "Emitting particles!\n";

	std::vector<Particle> ps(count);
	for (size_t i = 0; i < count; i++) {
		auto velDiv = M_PI * velDiviation / 180.0f;
		auto angle  = rnd() * velDiv + M_PI / 2.0f;

		ps[i].pos = pos;
		ps[i].vel = sf::Vector2f(cos(angle), sin(angle)) * 0.80f * (rnd() + 1.0f);
	}

	// Upload particles
	auto pg = new ParticleGroup(count, acc);

	// Make vertex array object
	glGenVertexArrays(1, &pg->vao);
	glBindVertexArray(pg->vao);

	// Make a vertex buffer and set the vertex format
	// Make vertex buffer object
	glGenBuffers(1, &pg->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pg->vbo);

	// Upload data
	glBufferData(GL_ARRAY_BUFFER, ps.size() * sizeof(Particle), ps.data(),
	             GL_STATIC_DRAW);

	// Specify format of the data
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
	                      nullptr); // Pos
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
	                      (GLvoid *)(2 * sizeof(float))); // Vel

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::cout << glewGetErrorString(glGetError()) << std::endl;

	particleGroups.push_back(pg);
}

int main() {
	// Setup random
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

	// Setup window
	sf::ContextSettings context(24, 8, 0, 4, 5,
	                            sf::ContextSettings::Attribute::Core |
	                                sf::ContextSettings::Attribute::Debug,
	                            true);

	sf::Window window(sf::VideoMode(windowx, windowy), "The Fountain",
	                  sf::Style::Default, context);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	auto s = window.getSettings();
	std::cout << "OpenGL Context info:"
	          << "\n\tVersion: " << s.majorVersion << ", " << s.minorVersion
	          << "\n\tDepth bits: " << s.depthBits
	          << "\n\tStencil bits: " << s.stencilBits
	          << "\n\tAntialiasing level: " << s.antialiasingLevel << std::endl;

	// Setup glew
	if (glewInit() != GLEW_OK)
		std::cout << "Error: GLEW Failed to initialize\n";

	// Setup opengl
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup shaders
	std::vector<std::string> paths = {"../src/fountain-gpu.vert",
	                                  "../src/fountain-gpu.geom",
	                                  "../src/fountain-gpu.frag"};
	ShaderProgram            program(paths);
	program.use();

	// Setup emitter
	ParticleEmitter emitter(program.getProgram());

	sf::Clock clock;
	clock.restart();

	// Game loop
	// *******************************************************************
	while (window.isOpen()) {
		float dt = clock.restart().asSeconds();

		// Event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed: window.close(); break;

				case sf::Event::Resized:
					glViewport(0, 0, event.size.width, event.size.height);
					break;

				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Space) {
						// Generate new particles
						emitter.emit(10000, sf::Vector2f(0.0f, 0.0f),
						             sf::Vector2f(0.0f, 0.5f),
						             sf::Vector2f(0.0f, -1.0f), 60.0f);
					}

				default: break;
			}
		}

		// Rendering
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		emitter.emit(1000, sf::Vector2f(0.0f, -1.0f), sf::Vector2f(0.2f, 1.0f),
		             sf::Vector2f(0.3f, -0.6f), 30.0f);
		emitter.draw(dt);

		window.display();
	}

	return EXIT_SUCCESS;
}