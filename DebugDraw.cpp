#include "DebugDraw.hpp"

DebugDraw::DebugDraw(sf::RenderTarget& target)
        : mTarget(target)
{
}

void DebugDraw::draw(const Aabb& aabb, const sf::Transform transform, sf::Color color)
{
        sf::RectangleShape shape(aabb.getSize());
        shape.setPosition(transform.transformPoint(0.0f, 0.0f));
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(1.0f);

        mTarget.draw(shape);
}

void DebugDraw::draw(const Circle& circle, const sf::Transform transform, sf::Color color)
{
        sf::CircleShape shape(circle.getRadius());
        shape.setOrigin(circle.getRadius(), circle.getRadius());
        shape.setPosition(transform.transformPoint(0.0f, 0.0f));
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(1.0f);

        mTarget.draw(shape);
}

void DebugDraw::draw(const Ray& ray, const sf::Transform transform, sf::Color color)
{
        sf::Vector2f position = transform.transformPoint(0.0f, 0.0f);
        sf::Vector2f direction = ray.getDirection() * ray.getLength();
        sf::Vertex line[] = {
                sf::Vertex(position, color),
                sf::Vertex(position + direction, color),
        };

        mTarget.draw(line, 2, sf::Lines);
}

void DebugDraw::draw(const Manifold& manifold, sf::Color color)
{
        if (manifold.colliding)
        {
                sf::Vector2f normal = manifold.normal * manifold.depth;
                sf::Vertex point[] = {
                        sf::Vertex(manifold.contact, sf::Color::Red),
                };

                sf::Vertex line[] = {
                        sf::Vertex(manifold.contact, color),
                        sf::Vertex(manifold.contact + normal, color),
                };

                mTarget.draw(line, 2, sf::Lines);
                mTarget.draw(point, 1, sf::Points);
        }
}

void DebugDraw::draw(const Raycast& raycast, sf::Color color)
{
        if (raycast.hit)
        {
                sf::Vector2f normal = raycast.normal * 10.0f;
                sf::Vertex point[] = {
                        sf::Vertex(raycast.contact, sf::Color::Red),
                };

                sf::Vertex line[] = {
                        sf::Vertex(raycast.contact, color),
                        sf::Vertex(raycast.contact + normal, color),
                };

                mTarget.draw(line, 2, sf::Lines);
                mTarget.draw(point, 1, sf::Points);
        }
}

void DebugDraw::draw(const Shape& shape, const sf::Transform transform, sf::Color color)
{
        if (shape.getType() == Type::Aabb)
        {
                draw(castShape<Aabb>(shape), transform, color);
        }
        else if (shape.getType() == Type::Circle)
        {
                draw(castShape<Circle>(shape), transform, color);
        }
}
