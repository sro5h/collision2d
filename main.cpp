#include "DebugDraw.hpp"
#include "Collision.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

const float speed = 1.0f;
const sf::Time delta = sf::microseconds(15625);

int main()
{
        sf::RenderWindow window(sf::VideoMode(400, 400), "App");
        DebugDraw debug(window);

        Circle::Ptr shapeA = std::make_unique<Circle>(30);
        sf::Transform positionA;
        positionA.translate(200, 200);
        Aabb::Ptr shapeB = std::make_unique<Aabb>(50, 60);
        sf::Transform positionB;
        positionB.translate(300, 300);
        Manifold m;
        Ray::Ptr ray = std::make_unique<Ray>(0, 1, 100);
        sf::Transform positionR;
        positionR.translate(100, 100);
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
                                positionA.translate(1.0f * speed, 0.0f);
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                                positionA.translate(-1.0f * speed, 0.0f);
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                                positionA.translate(0.0f, 1.0f * speed);
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                                positionA.translate(0.0f, -1.0f * speed);

                        m.solve(*shapeA, positionA, *shapeB, positionB);
                        r.solve(*ray, positionR, *shapeA, positionA);

                        if (m.colliding)
                        {
                                positionA.translate(-m.normal * m.depth);
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
