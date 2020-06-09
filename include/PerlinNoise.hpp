/*
** EPITECH PROJECT, 2020
** PerlinNoise.hpp
** File description:
**
*/

#ifndef SORTINGVISUALIZER_PERLINNOISE_HPP
#define SORTINGVISUALIZER_PERLINNOISE_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>
#include <random>
#include "math.hpp"

struct Layer {
    Layer(const sf::Color &from, float valueMin, float valueMax) : from(from), to(), valueMin(valueMin), valueMax(valueMax), lerping(false) {}
    Layer(const sf::Color &from, const sf::Color &to, float valueMin, float valueMax) : from(from), to(to), valueMin(valueMin), valueMax(valueMax), lerping(true) {}
    sf::Color to;
    sf::Color from;
    float valueMin;
    float valueMax;
    bool lerping;
};

class PerlinNoise {
public:
    PerlinNoise();
    void calculateNoise();
    float noise(int x, int y) const;
    const sf::VertexArray &getVertices() const;
    float getResolution() const;
    void setResolution(float resolution);
    float getUnit() const;
    void setUnit(float unit);
    const sf::Vector2i &getPosition() const;
    void setPosition(const sf::Vector2i &pos);
private:
    sf::VertexArray _vertices;
    std::vector<int> _permu;
    float _resolution{100.f};
    float _unit{1};
    sf::Vector2i _pos{0, 0};
    std::default_random_engine _engine;
};

#endif //SORTINGVISUALIZER_PERLINNOISE_HPP
