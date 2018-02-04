#include "DebugDraw.hpp"
#include "Shape.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

int main()
{
        sf::RenderWindow window(sf::VideoMode(400, 400), "App");
        DebugDraw debug(window);

        auto shapeA = std::make_unique<Circle>(30);
        shapeA->setPosition(20, 20);
        auto shapeB = std::make_unique<Aabb>(50, 60);
        shapeB->move(20, 20);

        while (window.isOpen())
        {
                sf::Event event;
                while (window.pollEvent(event))
                {
                        if (event.type == sf::Event::Closed)
                                window.close();
                }

                Manifold m(*shapeA, *shapeB);
                if (m.colliding)
                {
                        std::cout << "Collision occured!" << std::endl;
                }

                window.clear();
                debug.draw(*shapeA);
                debug.draw(*shapeB);
                debug.draw(m, sf::Color::Green);
                window.display();
        }

        return EXIT_SUCCESS;
}
