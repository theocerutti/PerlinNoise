#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "PerlinNoise.hpp"

int speed = 10;
float resolution = 10;
bool speedMore = false;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    PerlinNoise perlinNoise;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta < 0) {
                    if (perlinNoise.getResolution() - resolution > 0)
                        perlinNoise.setResolution(perlinNoise.getResolution() - resolution);
                } else
                    perlinNoise.setResolution(perlinNoise.getResolution() + resolution);
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
            perlinNoise.calculateNoise();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
            perlinNoise.setUnit(perlinNoise.getUnit() - 0.1f);
            perlinNoise.calculateNoise();
        }
        window.clear();
        window.draw(perlinNoise.getVertices());
        window.display();
    }
    return EXIT_SUCCESS;
}