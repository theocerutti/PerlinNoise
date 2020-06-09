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
        float lerp(float a, float b, float f);

        float clamp2bounds(float b1Min, float b1Max, float x);

        sf::Color lerpColor(const sf::Color &from, const sf::Color &to, float f);
    }
}


#endif //SORTINGVISUALIZER_MATH_HPP
