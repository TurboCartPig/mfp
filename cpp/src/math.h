#pragma once

#include <SFML/System.hpp>
#include <cmath>

/**
 * Map a value in a range to a value equally far between min and max in
 * different range.
 *
 * @tparam T        The type of the values in the ranges
 * @param  v        The value in the first range
 * @param  in_min   Lower limit of the first range
 * @param  in_max   Upper limit of the first range
 * @param  out_min  Lower limit of the second range
 * @param  out_max  Upper limit of the second range
 * @return          The value in the second range
 */
template <typename T>
T map_range(T v, T in_min, T in_max, T out_min, T out_max) {
	return (v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename T>
T signum(T a) {
	return a >= 0 ? 1 : -1;
}

/**
 * Returns the length of the given vector.
 */
float length(const sf::Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }

/**
 * Limit a vectors magnitude.
 *
 * @param v   The vector
 * @param len The max length of the vector
 * @return    The vector now with a magnitude shorter or equaly to len
 */
sf::Vector2f limit(const sf::Vector2f v, float len) {
	if (length(v) > len) {
		return len * v / length(v);
	} else {
		return v;
	}
}

/**
 * Calculate the dot product of two vectors.
 *
 * @param a The first vector
 * @param b The second vector
 * @return  The dot product
 */
float dot(const sf::Vector2f a, const sf::Vector2f b) {
	return a.x * b.x + a.y * b.y;
}

/**
 * Project vector a onto vector b.
 *
 * @param a The vector being projected
 * @param b The vector being projected onto
 * @return  The projection vector
 */
sf::Vector2f project(const sf::Vector2f a, const sf::Vector2f b) {
	return b * dot(a, b) / (length(b) * length(b));
}

/**
 * Operator overloading for vector pretty printing.
 * @param os  Output stream
 * @param vec Vector to print
 * @return    Output stream with vector pretty printed
 */
std::ostream &operator<<(std::ostream &os, const sf::Vector2f &vec) {
	return os << "[" << vec.x << "," << vec.y << "]";
}

/**
 * Are the parameters almost equal?
 *
 * @param x
 * @param y
 * @param ulp acceptable deviation
 * @return    are they?
 */
template <class T>
// typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
bool almost_equal(T x, T y, int ulp) {
	// the machine epsilon has to be scaled to the magnitude of the values used
	// and multiplied by the desired precision in ULPs (units in the last place)
	return std::fabs(x - y) <=
	           std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
	       // unless the result is subnormal
	       || std::fabs(x - y) < std::numeric_limits<T>::min();
}