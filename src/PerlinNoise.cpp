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

    initSimplex();
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
        case NoiseType::Simplex: return (simplexNoise(x, y));
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
        result += improvedNoiseImpl(x, y, 0) * amp;
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
    _frequency = my::math::clamp(_frequency, 0, 1024.f);
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

double dot(int const g[3], double x, double y)
{
    return g[0] * x + g[1] * y;
}

void PerlinNoise::initSimplex()
{
    int pSimplex[] = {151,160,137,91,90,15,
               131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
               190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
               88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
               77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
               102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
               135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
               5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
               223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
               129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
               251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
               49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
               138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};
    // To remove the need for index wrapping, double the permutation table length
    permSimplex = new int[512];
    for(int i = 0; i < 512; i++)
        permSimplex[i] = pSimplex[i & 255];
}

double PerlinNoise::simplexNoiseImpl(double x, double y) const
{
    int grad3[12][3] = {{1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0},
                        {1,0,1},{-1,0,1},{1,0,-1},{-1,0,-1},
                        {0,1,1},{0,-1,1},{0,1,-1},{0,-1,-1}};
    double xin = x;
    double yin = y;
    double n0, n1, n2; // Noise contributions from the three corners
    // Skew the input space to determine which simplex cell we're in
    double F2 = 0.5*(std::sqrt(3.0)-1.0);
    double s = (xin+yin)*F2; // Hairy factor for 2D
    int i = my::math::fastfloor(xin+s);
    int j = my::math::fastfloor(yin+s);
    double G2 = (3.0 - std::sqrt(3.0))/6.0;
    double t = (i+j)*G2;
    double X0 = i-t; // Unskew the cell origin back to (x,y) space
    double Y0 = j-t;
    double x0 = xin-X0; // The x,y distances from the cell origin
    double y0 = yin-Y0;
    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if(x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    else {i1=0; j1=1;} // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6
    double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
    double y2 = y0 - 1.0 + 2.0 * G2;
    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = permSimplex[ii + permSimplex[jj]] % 12;
    int gi1 = permSimplex[ii + i1 + permSimplex[jj + j1]] % 12;
    int gi2 = permSimplex[ii + 1 + permSimplex[jj + 1]] % 12;
    // Calculate the contribution from the three corners
    double t0 = 0.5 - x0*x0-y0*y0;
    if(t0<0) n0 = 0.0;
    else {
        t0 *= t0;
        n0 = t0 * t0 * ::dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
    }
    double t1 = 0.5 - x1*x1-y1*y1;
    if(t1<0) n1 = 0.0;
    else {
        t1 *= t1;
        n1 = t1 * t1 * ::dot(grad3[gi1], x1, y1);
    }
    double t2 = 0.5 - x2*x2-y2*y2;
    if(t2<0) n2 = 0.0;
    else {
        t2 *= t2;
        n2 = t2 * t2 * ::dot(grad3[gi2], x2, y2);
    }
    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [0,1].
    return ((70.0 * (n0 + n1 + n2)) + 1.f) / 2.f;
}

double PerlinNoise::simplexNoise(double x, double y) const
{
    double output = 0.f;
    double denom  = 0.f;
    double frequency = _frequency;
    double amplitude = _amplitude;
    double lacunarity = _lacunarity;
    double persistence = _persistence;

    for (size_t i = 0; i < _octaves; i++) {
        output += (amplitude * simplexNoiseImpl((x / 800) * frequency, (y / 600)  * frequency));
        denom += amplitude;

        frequency *= lacunarity;
        amplitude *= persistence;
    }

    return (output / denom);
}