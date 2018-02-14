#include "DebugDraw.hpp"
#include "Shape.hpp"
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
        shapeA->setPosition(100, 100);
        Aabb::Ptr shapeB = std::make_unique<Aabb>(50, 60);
        shapeB->move(180, 180);
        Manifold m;
        Ray::Ptr ray = std::make_unique<Ray>(0, 1, 100);
        ray->setPosition(50, 50);
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

                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                                shapeA->move(-1.0f * speed, 0);
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                                shapeA->move( 1.0f * speed, 0);
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                                shapeA->move(0, -1.0f * speed);
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                                shapeA->move(0,  1.0f * speed);

                        m.solve(*shapeA, *shapeB);
                        r.solve(*ray, *shapeA);

                        if (m.colliding)
                        {
                                shapeA->move(-m.normal * m.depth);
                        }
                }

                window.clear();
                debug.draw(*shapeA);
                debug.draw(*shapeB);
                debug.draw(*ray);
                debug.draw(m, sf::Color::Green);
                debug.draw(r, sf::Color::Cyan);
                window.display();
        }

        return EXIT_SUCCESS;
}
