#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

/// Vector dot product
float dot(sf::Vector3f a, sf::Vector3f b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// Vector cross product
sf::Vector3f cross(sf::Vector3f a, sf::Vector3f b) {
	return sf::Vector3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
	                    a.x * b.y - a.y * b.x);
}

/// A Quaternion
class Quat {
  public:
	Quat(const float c_in, const sf::Vector3f u_in) : c(c_in), u(u_in) {}
	Quat         operator*(const Quat &other);
	sf::Vector3f operator*(const sf::Vector3f &other);

	float        c;
	sf::Vector3f u;
};

/**
 * Quaternion dot product.
 *
 * @param other The right operand
 * @return      The dot product
 */
Quat Quat::operator*(const Quat &other) {
	return Quat(c * other.c - dot(u, other.u),
	            c * other.u + other.c * u + cross(u, other.u));
}

sf::Vector3f Quat::operator*(const sf::Vector3f &other) {
	Quat vq(0.0f, other);
	auto p = *this * vq * Quat(c, -1.0f * u);
	return p.u;
}

/// Operator overload to support printing Quats
std::ostream &operator<<(std::ostream &os, const Quat &q) {
	return os << "(" << q.c << ", [" << q.u.x << "," << q.u.y << "," << q.u.z
	          << "])";
}

std::ostream &operator<<(std::ostream &os, const sf::Vector3f &v) {
	return os << "[" << v.x << "," << v.y << "," << v.z << "]";
}

int main() {
	Quat i = {0.0f, sf::Vector3f(1.0f, 0.0f, 0.0f)};
	Quat j = {0.0f, sf::Vector3f(0.0f, 1.0f, 0.0f)};
	Quat k = {0.0f, sf::Vector3f(0.0f, 0.0f, 1.0f)};

	std::cout << "ii: " << (i * i) << std::endl;
	std::cout << "jj: " << (j * j) << std::endl;
	std::cout << "kk: " << (k * k) << std::endl;

	//    std::cout << "ij: " << (dot(i, j)) << std::endl;
	//    std::cout << "ji: " << (dot(j, i)) << std::endl;
	//    std::cout << "ik: " << (dot(i, k)) << std::endl;
	//    std::cout << "ki: " << (dot(k, i)) << std::endl;
	//    std::cout << "jk: " << (dot(j, k)) << std::endl;
	//    std::cout << "kj: " << (dot(k, j)) << std::endl;

	// Should be [-1.0f, 1.0f, 0.0f]
	std::cout << "[1,1,0] 90 Deg: "
	          << (Quat(cos(M_PI / 4.0f), (float)sin(M_PI / 4.0f) *
	                                         sf::Vector3f(0.0f, 0.0f, 1.0f)) *
	              sf::Vector3f(1.0f, 1.0f, 0.0f))
	          << std::endl;

	return EXIT_SUCCESS;
}