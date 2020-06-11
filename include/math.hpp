/*
** EPITECH PROJECT, 2020
** math.hpp
** File description:
**
*/

#ifndef SORTINGVISUALIZER_MATH_HPP
#define SORTINGVISUALIZER_MATH_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace my {
    namespace math {
        double constexpr lerp(double a, double b, double f)
        {
            return a + f * (b - a);
        }

        double inline constexpr clamp2bounds(double b1Min, double b1Max, double x)
        {
            double diff = b1Min;
            double newMax = b1Max - diff;
            double newX = x - diff;

            return ((1 / newMax) * newX);
        }

        sf::Color lerpColor(const sf::Color &from, const sf::Color &to, double f);

        double constexpr fade(double t)
        {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }

        double constexpr grad(int hash, double x, double y, double z)
        {
            const std::uint8_t h = hash & 15;
            const double u = h < 8 ? x : y;
            const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }

        double constexpr clamp(double n, double lower, double upper)
        {
            return std::max(lower, std::min(n, upper));
        }

        int constexpr fastfloor(double x) {
            return (x > 0 ? (int)x : (int)x - 1);
        }
    }
}

#endif //SORTINGVISUALIZER_MATH_HPP
