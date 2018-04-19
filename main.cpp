#include "DebugDraw.hpp"
#include "Collision.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

template <typename T>
using uptr = std::unique_ptr<T>;

const float speed = 1.0f;
const sf::Time delta = sf::microseconds(15625);

int main()
{
        sf::RenderWindow window(sf::VideoMode(400, 400), "App");
        DebugDraw debug(window);

        uptr<Circle> shapeA = std::make_unique<Circle>(30);
        sf::Vector2f positionA(200, 200);
        uptr<Aabb> shapeB = std::make_unique<Aabb>(50, 60);
        sf::Vector2f positionB(300, 300);
        Manifold m;

        uptr<Ray> ray = std::make_unique<Ray>(0, 1, 100);
        sf::Vector2f positionR(100, 100);
        Raycast r;

        sf::Clock clock;
        sf::Time accumulator;

        while (window.isOpen())
        {
                accumulator += clock.restart();

                sf::Event event;
                while (window.pollEvent(event))
                {
                        if (event.type == sf::Event::Closed)
                                window.close();
                }

                while (accumulator >= delta)
                {
                        accumulator -= delta;

                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                                positionA.x += 1.0f * speed;
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                                positionA.x -= 1.0f * speed;
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                                positionA.y += 1.0f * speed;
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                                positionA.y -= 1.0f * speed;

                        m = solve(*shapeA, positionA, *shapeB, positionB);
                        r = solve(*ray, positionR, *shapeA, positionA);

                        if (m.colliding)
                        {
                                positionA -= m.normal * m.depth;
                        }
                }

                window.clear();
                debug.draw(*shapeA, positionA);
                debug.draw(*shapeB, positionB);
                debug.draw(*ray, positionR);
                debug.draw(m, sf::Color::Green);
                debug.draw(r, sf::Color::Cyan);
                window.display();
        }

        return EXIT_SUCCESS;
}
