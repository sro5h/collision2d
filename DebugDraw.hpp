#ifndef DEBUG_DRAW_HPP_INCLUDED
#define DEBUG_DRAW_HPP_INCLUDED

#include "Collision.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class DebugDraw final
{
public:
        DebugDraw(sf::RenderTarget& target);

        void draw(const Aabb& aabb, const sf::Vector2f position,
                        sf::Color color = sf::Color::White);
        void draw(const Circle& circle, const sf::Vector2f position,
                        sf::Color color = sf::Color::White);
        void draw(const Ray& ray, const sf::Vector2f position,
                        sf::Color color = sf::Color::White);
        void draw(const Manifold& manifold, sf::Color color = sf::Color::White);
        void draw(const Raycast& raycast, sf::Color color = sf::Color::White);
        void draw(const Shape& shape, const sf::Vector2f position,
                        sf::Color color = sf::Color::White);

private:
        sf::RenderTarget& mTarget;
};

#endif // DEBUG_DRAW_HPP_INCLUDED
