#ifndef SHAPE_HPP
#define SHAPE_HPP

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
        typedef std::unique_ptr<Shape> Ptr;

public:
        virtual ~Shape() = default;

        Type getType() const;
        sf::Vector2f getPosition() const;
        void setPosition(sf::Vector2f position);
        void setPosition(float x, float y);
        void move(sf::Vector2f amount);
        void move(float x, float y);

protected:
        Shape(Type type);

private:
        const Type mType;
        sf::Vector2f mPosition;
};

class Circle : public Shape
{
public:
        typedef std::unique_ptr<Circle> Ptr;

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
        typedef std::unique_ptr<Aabb> Ptr;

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
        typedef std::unique_ptr<Ray> Ptr;

public:
        Ray(sf::Vector2f direction, float length);
        Ray(float x, float y, float length);

        sf::Vector2f getPosition() const;
        void setPosition(sf::Vector2f position);
        void setPosition(float x, float y);
        sf::Vector2f getDirection() const;
        void setDirection(sf::Vector2f direction);
        void setDirection(float x, float y);
        float getLength() const;
        void setLength(float length);

private:
        sf::Vector2f mPosition;
        sf::Vector2f mDirection;
        float mLength;
};

class Manifold
{
public:
        Manifold();

        void solve(const Shape& a, const Shape& b);

private:
        void dispatch(const Shape& a, const Shape& b);
        void collide(const Aabb& a, const Aabb& b);
        void collide(const Circle& a, const Circle& b);
        void collide(const Aabb& a, const Circle& b);
        void collide(const Circle& a, const Aabb& b);

public:
        bool colliding;
        sf::Vector2f normal;
        sf::Vector2f contact;
        float depth;
};

class Raycast
{
public:
        Raycast();

        void solve(const Ray& a, const Shape& b);

private:
        void dispatch(const Ray& a, const Shape& b);
        void calculate(const Ray& a, const Aabb& b);
        void calculate(const Ray& a, const Circle& b);

public:
        bool hit;
        sf::Vector2f normal;
        sf::Vector2f contact;
        float t;
};

template<typename T>
const T& castShape(const Shape& shape)
{
        assert(dynamic_cast<const T*>(&shape) == &shape);
        return *static_cast<const T*>(&shape);
}

#endif // SHAPE_HPP
