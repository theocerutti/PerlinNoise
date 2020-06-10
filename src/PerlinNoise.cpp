/*
** EPITECH PROJECT, 2020
** PerlinNoise.hpp
** File description:
**
*/

#include "PerlinNoise.hpp"

PerlinNoise::PerlinNoise() : _engine(std::chrono::system_clock::now().time_since_epoch().count())
{
    std::uniform_int_distribution<int> distribution{0, 255};
    _permu.reserve(256);
    for (int i = 0; i < 256; i++)
        _permu[i] = distribution(_engine);
    calculateNoise();
}

std::vector<Layer> colorLayers{
        // water
        {sf::Color(0, 0,255), sf::Color(0, 190, 230), 0, 0.5},
        // beach
        {sf::Color(210,180,140),  sf::Color(238, 214, 175), 0.5, 0.55},
        // green
        {sf::Color(34,139,34), sf::Color(0,100,0), 0.55, 0.7},
        // moutain
        {sf::Color(139, 137, 137), sf::Color(170, 170, 170), 0.7, 0.9},
        // neige
        {sf::Color(180, 180, 180), sf::Color(255, 255, 255), 0.9, 1.0}
};

void PerlinNoise::calculateNoise()
{
    _vertices.clear();
    for (int x = 0; x < 800; x++) {
        for (int y = 0; y < 600; y++) {
            float perlinValue = noise(x + _pos.x, y + _pos.y);
            perlinValue = (perlinValue + 1.f) / 2.f;
            sf::Color colorVertex;
            for (auto &layer : colorLayers) {
                if (layer.valueMin <= perlinValue && perlinValue <= layer.valueMax) {
                    if (layer.lerping) {
                        colorVertex = my::math::lerpColor(layer.from, layer.to, my::math::clamp2bounds(layer.valueMin, layer.valueMax, perlinValue));
                    } else {
                        colorVertex = layer.from;
                    }
                    break;
                }
            }
            sf::Vertex vertex(sf::Vector2f(x, y), colorVertex);
            _vertices.append(vertex);
        }
    }
}

float PerlinNoise::noise(int x, int y) const
{
    float g[][2] = {{_unit, _unit}, {-_unit, _unit}, {_unit, -_unit}, {-_unit, -_unit},
                    {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    sf::Vector2f a = {x / _resolution, y / _resolution};
    sf::Vector2i c = {(int)a.x, (int)a.y};
    sf::Vector2i mask = {c.x & 255, c.y & 255};
    int v0 = _permu[mask.x + _permu[mask.y]] % 8;
    int v1 = _permu[mask.x + 1 + _permu[mask.y]] % 8;
    int v2 = _permu[mask.x + _permu[mask.y + 1]] % 8;
    int v3 = _permu[mask.x + 1 + _permu[mask.y + 1]] % 8;
    float vt0 = g[v0][0] * (a.x - c.x) + g[v0][1] * (a.y - c.y);
    float vt1 = g[v1][0] * (a.x - (c.x + 1.f)) + g[v1][1] * (a.y - c.y);
    float vt2 = g[v2][0] * (a.x - c.x) + g[v2][1] * (a.y - (c.y + 1.f));
    float vt3 = g[v3][0] * (a.x - (c.x + 1.f)) + g[v3][1] * (a.y - (c.y + 1.f));
    sf::Vector2f temp = {a.x - c.x, a.y - c.y};
    float cx = 3.f * (temp.x * temp.x) - 2 * (temp.x * temp.x * temp.x);
    float cy = 3.f * (temp.y * temp.y) - 2 * (temp.y * temp.y * temp.y);
    sf::Vector2f li = {vt0 + cx * (vt1 - vt0), vt2 + cx * (vt3 - vt2)};

    float noise = li.x + cy * (li.y - li.x);

    if (noise < -1)
        noise = -1;
    else if (noise > 1)
        noise = 1;
    return noise;
}

const sf::VertexArray &PerlinNoise::getVertices() const
{
    return (_vertices);
}

float PerlinNoise::getResolution() const
{
    return (_resolution);
}

void PerlinNoise::setResolution(float resolution)
{
    _resolution = resolution;
}

float PerlinNoise::getUnit() const
{
    return (_unit);
}

void PerlinNoise::setUnit(float unit)
{
    _unit = unit;
}

const sf::Vector2i &PerlinNoise::getPosition() const
{
    return (_pos);
}

void PerlinNoise::setPosition(const sf::Vector2i &pos)
{
    _pos = pos;
}