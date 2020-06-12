#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "PerlinNoise.hpp"

int speed = 10;
float resolution = 10;
bool speedMore = false;

int main()
{
    sf::Clock deltaClock;
    PerlinNoise perlinNoise(sf::Vector2u(800, 600));
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    /*perlinNoise.setLayers({
        // water
        {sf::Color(0, 0,255), sf::Color(0, 190, 230), 0, 0.6},
        // beach
        {sf::Color(210,180,140),  sf::Color(238, 214, 175), 0.6, 0.65},
        // green
        {sf::Color(34,139,34), sf::Color(0,100,0), 0.65, 0.75},
        // dirt
        {sf::Color(90, 50, 15), sf::Color(68, 39, 12), 0.75, 0.83},
        // moutain
        {sf::Color(139, 137, 137), sf::Color(170, 170, 170), 0.83, 0.9},
        // neige
        {sf::Color(180, 180, 180), sf::Color(255, 255, 255), 0.9, 1.0}
    });*/

    perlinNoise.setNoiseType(NoiseType::Improved);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta < 0) {
                    if (perlinNoise.getResolution() - resolution > 0)
                        perlinNoise.setResolution(perlinNoise.getResolution() - resolution);
                    perlinNoise.setFrequency(perlinNoise.getFrequency() + 0.5f);
                } else {
                    perlinNoise.setResolution(perlinNoise.getResolution() + resolution);
                    perlinNoise.setFrequency(perlinNoise.getFrequency() - 0.5f);
                }
                perlinNoise.calculateNoise();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::LShift)
                    speedMore = true;
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::LShift)
                    speedMore = false;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            perlinNoise.setPosition(sf::Vector2i(perlinNoise.getPosition().x + (speedMore ? speed * 2 : speed), perlinNoise.getPosition().y));
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            perlinNoise.setPosition(sf::Vector2i(perlinNoise.getPosition().x - (speedMore ? speed * 2 : speed), perlinNoise.getPosition().y));
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            perlinNoise.setPosition(sf::Vector2i(perlinNoise.getPosition().x, perlinNoise.getPosition().y + (speedMore ? speed * 2 : speed)));
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            perlinNoise.setPosition(sf::Vector2i(perlinNoise.getPosition().x, perlinNoise.getPosition().y - (speedMore ? speed * 2 : speed)));
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            perlinNoise.setUnit(perlinNoise.getUnit() + 0.1f);
            perlinNoise.setOctaves(perlinNoise.getOctaves() + 1);
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
            perlinNoise.setUnit(perlinNoise.getUnit() - 0.1f);
            perlinNoise.setOctaves(perlinNoise.getOctaves() - 1);
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            perlinNoise._amplitude += 0.5f;
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
            perlinNoise._amplitude -= 0.5f;
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            perlinNoise._lacunarity += 0.5f;
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
            perlinNoise._lacunarity -= 0.5f;
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
            perlinNoise._persistence -= 0.1f;
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
            perlinNoise._persistence += 0.1f;
            perlinNoise.calculateNoise();
        }
        if (perlinNoise.needToRedraw()) {
            window.clear();
            window.draw(perlinNoise.getVertices(), perlinNoise.getVertexCount(), sf::Points);
        }
        window.display();
    }
    return EXIT_SUCCESS;
}