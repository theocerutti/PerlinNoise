/*
** EPITECH PROJECT, 2020
** math.hpp
** File description:
**
*/

#include "math.hpp"

float my::math::lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

float my::math::clamp2bounds(float b1Min, float b1Max, float x)
{
    float diff = b1Min;
    float newMax = b1Max - diff;
    float newX = x - diff;

    return ((1 / newMax) * newX);
}

sf::Color my::math::lerpColor(const sf::Color &from, const sf::Color &to, float f)
{
    return (sf::Color(
            lerp(from.r, to.r, f),
            lerp(from.g, to.g, f),
            lerp(from.b, to.b, f)
    ));
}