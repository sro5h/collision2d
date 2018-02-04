#include "DebugDraw.hpp"

DebugDraw::DebugDraw(sf::RenderTarget& target)
        : mTarget(target)
{
}

void DebugDraw::draw(const Aabb& aabb, sf::Color color)
{
        sf::RectangleShape shape(aabb.getSize());
        shape.setPosition(aabb.getPosition());
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(1.0f);

        mTarget.draw(shape);
}

void DebugDraw::draw(const Circle& circle, sf::Color color)
{
        sf::CircleShape shape(circle.getRadius());
        shape.setOrigin(circle.getRadius(), circle.getRadius());
        shape.setPosition(circle.getPosition());
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(1.0f);

        mTarget.draw(shape);
}
