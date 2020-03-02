#pragma once

#include "SFML/Graphics.hpp"
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
template <class T>
T map_range(T v, T in_min, T in_max, T out_min, T out_max) {
    return (v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * Returns the length of the given vector.
 */
float length(const sf::Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }

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
    // auto aLen = length(a);
    // auto cosine = dot(a, b) / aLen * length(b);
    // return aLen * cosine * b;

    // auto d     = dot(a, b);
    // auto blen  = length(b);
    // auto anorm = a / length(a);
    // auto proj  = b * d / (blen * blen);
    // return proj;

    // Remember to surround the length in parenthesis
    // With: Divide vector
    // Without: Multiply with vector
    return b * dot(a, b) / (length(b) * length(b));
}