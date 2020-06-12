/*
** EPITECH PROJECT, 2020
** math.hpp
** File description:
**
*/

#include "math.hpp"

sf::Color my::math::lerpColor(const sf::Color &from, const sf::Color &to, double f)
{
    return (sf::Color(
            lerp(from.r, to.r, f),
            lerp(from.g, to.g, f),
            lerp(from.b, to.b, f)
    ));
}

void my::math::fastLerpColor(sf::Color &result, const sf::Color &from, const sf::Color &to, double f)
{
    result.r = lerp(from.r, to.r, f);
    result.g = lerp(from.g, to.g, f);
    result.b = lerp(from.b, to.b, f);
}