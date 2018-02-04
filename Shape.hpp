#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <SFML/System/Vector2.hpp>

enum class Type
{
        Aabb,
        Circle,
};

class Shape
{
public:
        virtual ~Shape() = 0;

        sf::Vector2f getPosition() const;
        void setPosition(sf::Vector2f position);
        void setPosition(float x, float y);
        void move(sf::Vector2f amount);
        void move(float x, float y);

protected:
        Shape(Type type);

private:
        Type mType;
        sf::Vector2f mPosition;

        friend class Manifold;
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

class Manifold
{
public:
        Manifold(const Shape& a, const Shape& b);

private:
        void dispatch(const Shape& a, const Shape& b);
        void solve(const Aabb& a, const Aabb& b);
        void solve(const Circle& a, const Circle& b);
        void solve(const Aabb& a, const Circle& b);
        void solve(const Circle& a, const Aabb& b);

public:
        bool colliding;
        sf::Vector2f normal;
        sf::Vector2f contact;
        float depth;
};

#endif // SHAPE_HPP
