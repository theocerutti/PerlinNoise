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

enum class NoiseType {
    Classic,
    Improved,
    Simplex
};

class PerlinNoise {
public:
    PerlinNoise(const sf::Vector2u &size, std::uint32_t seed);
    PerlinNoise(const sf::Vector2u &size);
    void calculateNoise();
    double noiseFromType(int x, int y) const;
    float classicNoise2D(int x, int y) const;

    double improvedNoise2D(double x, double y) const;
    double improvedNoiseImpl(double x, double y, double z) const;

    double simplexNoise(double x, double y) const;
    double simplexNoiseImpl(double x, double y) const;
    void initSimplex();

    const sf::VertexArray &getVertices() const;

    float getResolution() const;
    void setResolution(float resolution);

    float getUnit() const;
    void setUnit(float unit);

    const sf::Vector2i &getPosition() const;
    void setPosition(const sf::Vector2i &pos);
    int getOctaves() const;
    void setOctaves(int octaves);

    double getFrequency() const;
    void setFrequency(double frequency);

    void setNoiseType(const NoiseType &type);
    const NoiseType &getNoiseType() const;

    void addLayer(const Layer &layer);
    void setLayers(const std::vector<Layer> &layers);
    const std::vector<Layer> &getLayers() const;
private:
    sf::Vector2u _size;
    sf::VertexArray _vertices;
    float _resolution{100.f}; // zoom
    float _unit{1};
    sf::Vector2i _pos{0, 0};
    std::default_random_engine _engine;

    std::uint8_t _p[512]{0};
    int _octaves{8}; // details
    double _frequency{16}; // zoom
    std::uint32_t _seed;
    NoiseType _type{NoiseType::Classic};
    std::vector<Layer> _layers;

    int *permSimplex;
public:
    double _amplitude{0.5};
    double _lacunarity{0.5};
    double _persistence{0.5};
};

#endif //SORTINGVISUALIZER_PERLINNOISE_HPP
