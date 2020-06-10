/*
** EPITECH PROJECT, 2020
** PerlinNoise.hpp
** File description:
**
*/

#include "PerlinNoise.hpp"

PerlinNoise::PerlinNoise(const sf::Vector2u &size, std::uint32_t seed)
    : _size(size),
      _engine(std::chrono::system_clock::now().time_since_epoch().count()),
      _seed(seed)
{
    for (size_t i = 0; i < 256; ++i)
        _p[i] = static_cast<std::uint8_t>(i);
    std::shuffle(std::begin(_p), std::begin(_p) + 256, std::default_random_engine(_seed));
    for (size_t i = 0; i < 256; ++i)
        _p[256 + i] = _p[i];

    calculateNoise();
}

PerlinNoise::PerlinNoise(const sf::Vector2u &size) : PerlinNoise(size, std::chrono::system_clock::now().time_since_epoch().count())
{
}

void PerlinNoise::calculateNoise()
{
    _vertices.clear();
    for (int x = 0; x < _size.x; x++) {
        for (int y = 0; y < _size.y; y++) {
            double perlinValue = noiseFromType(x + _pos.x, y + _pos.y);
            sf::Color colorVertex;
            if (_layers.empty()) {
                colorVertex = my::math::lerpColor(sf::Color::White, sf::Color::Black, perlinValue);
            } else {
                for (auto &layer : _layers) {
                    if (layer.valueMin <= perlinValue && perlinValue <= layer.valueMax) {
                        if (layer.lerping) {
                            colorVertex = my::math::lerpColor(layer.from, layer.to, my::math::clamp2bounds(layer.valueMin, layer.valueMax, perlinValue));
                        } else
                            colorVertex = layer.from;
                        break;
                    }
                }
            }
            sf::Vertex vertex(sf::Vector2f(x, y), colorVertex);
            _vertices.append(vertex);
        }
    }
}

double PerlinNoise::noiseFromType(int x, int y) const
{
    switch (_type) {
        case NoiseType::Classic: return (classicNoise2D(x, y));
        case NoiseType::Improved: return (improvedNoise2D(x, y));
        case NoiseType::Simplex: return (0);
        default: return (0);
    }
}

float PerlinNoise::classicNoise2D(int x, int y) const
{
    float g[][2] = {{_unit, _unit}, {-_unit, _unit}, {_unit, -_unit}, {-_unit, -_unit},
                    {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    sf::Vector2f a = {x / _resolution, y / _resolution};
    sf::Vector2i c = {(int)a.x, (int)a.y};
    sf::Vector2i mask = {c.x & 255, c.y & 255};
    int v0 = _p[mask.x + _p[mask.y]] % 8;
    int v1 = _p[mask.x + 1 + _p[mask.y]] % 8;
    int v2 = _p[mask.x + _p[mask.y + 1]] % 8;
    int v3 = _p[mask.x + 1 + _p[mask.y + 1]] % 8;
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
    return (noise + 1.f) / 2.f;
}

double PerlinNoise::improvedNoise2D(double x, double y) const
{
    x /= (_size.x / _frequency);
    y /= (_size.y / _frequency);
    double result = 0;
    double amp = 1;

    for (std::int32_t i = 0; i < _octaves; ++i) {
        result += improvedNoiseImpl(x, y, 2) * amp;
        x *= 2;
        y *= 2;
        amp /= 2;
    }
    return (my::math::clamp((result + 1.f) / 2.f, 0, 1));
}

double PerlinNoise::improvedNoiseImpl(double x, double y, double z) const
{
    const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
    const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
    const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    const double u = my::math::fade(x);
    const double v =  my::math::fade(y);
    const double w =  my::math::fade(z);

    const std::int32_t A = _p[X] + Y, AA = _p[A] + Z, AB = _p[A + 1] + Z;
    const std::int32_t B = _p[X + 1] + Y, BA = _p[B] + Z, BB = _p[B + 1] + Z;

    return my::math::lerp(w, my::math::lerp(my::math::lerp(my::math::grad(_p[AA], x, y, z),
                                               my::math::grad(_p[BA], x - 1, y, z), u),
                                               my::math::lerp(my::math::grad(_p[AB], x, y - 1, z),
                                               my::math::grad(_p[BB], x - 1, y - 1, z), u), v),
                                               my::math::lerp(my::math::lerp(my::math::grad(_p[AA + 1], x, y, z - 1),
                                               my::math::grad(_p[BA + 1], x - 1, y, z - 1), u),
                                               my::math::lerp(my::math::grad(_p[AB + 1], x, y - 1, z - 1),
                                               my::math::grad(_p[BB + 1], x - 1, y - 1, z - 1), u), v));
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

int PerlinNoise::getOctaves() const
{
    return (_octaves);
}

void PerlinNoise::setOctaves(int octaves)
{
    _octaves = octaves;
    _octaves = (int)my::math::clamp(_octaves, 1, 16);
}

double PerlinNoise::getFrequency() const
{
    return (_frequency);
}

void PerlinNoise::setFrequency(double frequency)
{
    _frequency = frequency;
    _frequency = my::math::clamp(_frequency, 0.1, 64.f);
}

const NoiseType &PerlinNoise::getNoiseType() const
{
    return (_type);
}

void PerlinNoise::setNoiseType(const NoiseType &type)
{
    _type = type;
    calculateNoise();
}

void PerlinNoise::addLayer(const Layer &layer)
{
    _layers.push_back(layer);
}

const std::vector<Layer> &PerlinNoise::getLayers() const
{
    return (_layers);
}

void PerlinNoise::setLayers(const std::vector<Layer> &layers)
{
    _layers = layers;
}
