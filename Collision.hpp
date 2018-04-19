#ifndef COLLISION_HPP_INCLUDED
#define COLLISION_HPP_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <memory>
#include <cassert>

enum class Type
{
        Aabb,
        Circle,
};

class Shape
{
public:
        virtual ~Shape() = default;

        Type getType() const;

protected:
        Shape(Type type);

private:
        const Type mType;
};

class Circle : public Shape
{
public:
        Circle(float radius);

        float getRadius() const;
        void setRadius(float radius);

private:
        float mRadius;
};

class Aabb : public Shape
{
public:
        Aabb(sf::Vector2f size);
        Aabb(float width, float height);

        sf::Vector2f getSize() const;
        void setSize(sf::Vector2f size);
        void setSize(float width, float height);

private:
        sf::Vector2f mSize;
};

class Ray
{
public:
        Ray(sf::Vector2f direction, float length);
        Ray(float x, float y, float length);

        sf::Vector2f getDirection() const;
        void setDirection(sf::Vector2f direction);
        void setDirection(float x, float y);
        float getLength() const;
        void setLength(float length);

private:
        sf::Vector2f mDirection;
        float mLength;
};

struct Manifold
{
        Manifold();

        bool colliding;
        sf::Vector2f normal;
        sf::Vector2f contact;
        float depth;
};

struct Raycast
{
        Raycast();

        bool hit;
        sf::Vector2f normal;
        sf::Vector2f contact;
        float t;
};

Manifold solve(const Shape& a, sf::Vector2f posA, const Shape& b,
               sf::Vector2f posB);
Raycast solve(const Ray& a, sf::Vector2f posA, const Shape& b,
               sf::Vector2f posB);

template<typename T>
const T& castShape(const Shape& shape)
{
        assert(dynamic_cast<const T*>(&shape) == &shape);
        return *static_cast<const T*>(&shape);
}

#endif // COLLISION_HPP_INCLUDED
