#include "Shape.hpp"
#define TINYC2_IMPLEMENTATION
#include "tinyc2.h"
#include <cassert>

void handleResult(const c2Manifold&, Manifold&);
c2AABB shapeToc2(const Aabb&);
c2Circle shapeToc2(const Circle&);
c2v toc2v(const sf::Vector2f&);

Shape::Shape(Type type)
        : mType(type)
        , mPosition(0, 0)
{
}

Shape::~Shape()
{
}

sf::Vector2f Shape::getPosition() const
{
        return mPosition;
}

void Shape::setPosition(sf::Vector2f position)
{
        mPosition = position;
}

void Shape::setPosition(float x, float y)
{
        mPosition.x = x;
        mPosition.y = y;
}

void Shape::move(sf::Vector2f amount)
{
        mPosition += amount;
}

void Shape::move(float x, float y)
{
        mPosition.x += x;
        mPosition.y += y;
}

Circle::Circle(float radius)
        : Shape(Type::Circle)
        , mRadius(radius)
{
}

float Circle::getRadius() const
{
        return mRadius;
}

void Circle::setRadius(float radius)
{
        mRadius = radius;
}

Aabb::Aabb(sf::Vector2f size)
        : Shape(Type::Aabb)
        , mSize(size)
{
}

Aabb::Aabb(float width, float height)
        : Shape(Type::Aabb)
        , mSize(width, height)
{
}

sf::Vector2f Aabb::getSize() const
{
        return mSize;
}

void Aabb::setSize(sf::Vector2f size)
{
        mSize = size;
}

void Aabb::setSize(float width, float height)
{
        mSize.x = width;
        mSize.y = height;
}

Manifold::Manifold(const Shape& a, const Shape& b)
{
        dispatch(a, b);
}

template<typename T>
const T& castShape(const Shape& shape)
{
        assert(dynamic_cast<const T*>(&shape) == &shape);
        return *static_cast<const T*>(&shape);
}

void Manifold::dispatch(const Shape& a, const Shape& b)
{
        if (a.mType == Type::Aabb)
        {
                const Aabb& aChild = castShape<Aabb>(a);

                if (b.mType == Type::Aabb)
                {
                        const Aabb& bChild = castShape<Aabb>(b);
                        solve(aChild, bChild);
                }
                else if (b.mType == Type::Circle)
                {
                        const Circle& bChild = castShape<Circle>(b);
                        solve(aChild, bChild);
                }
        }
        else if (a.mType == Type::Circle)
        {
                const Circle& aChild = castShape<Circle>(a);

                if (b.mType == Type::Aabb)
                {
                        const Aabb& bChild = castShape<Aabb>(b);
                        solve(aChild, bChild);
                }
                else if (b.mType == Type::Circle)
                {
                        const Circle& bChild = castShape<Circle>(b);
                        solve(aChild, bChild);
                }
        }
}

void Manifold::solve(const Aabb& a, const Aabb& b)
{
        c2AABB shapeA = shapeToc2(a);
        c2AABB shapeB = shapeToc2(b);
        c2Manifold m;
        c2AABBtoAABBManifold(shapeA, shapeB, &m);

        handleResult(m, *this);
}

void Manifold::solve(const Circle& a, const Circle& b)
{
        c2Circle shapeA = shapeToc2(a);
        c2Circle shapeB = shapeToc2(b);
        c2Manifold m;
        c2CircletoCircleManifold(shapeA, shapeB, &m);

        handleResult(m, *this);
}

void Manifold::solve(const Aabb& a, const Circle& b)
{
        solve(b, a);
        normal = -normal;
}

void Manifold::solve(const Circle& a, const Aabb& b)
{
        c2Circle shapeA = shapeToc2(a);
        c2AABB shapeB = shapeToc2(b);
        c2Manifold m;
        c2CircletoAABBManifold(shapeA, shapeB, &m);

        handleResult(m, *this);
}

void handleResult(const c2Manifold& manifold, Manifold& m)
{
        m.colliding = manifold.count > 0;
        if (m.colliding)
        {
                m.normal.x = manifold.normal.x;
                m.normal.y = manifold.normal.y;
                m.depth = manifold.depths[0];
        }
}

c2AABB shapeToc2(const Aabb& aabb)
{
        c2AABB tmp;
        tmp.min = toc2v(aabb.getPosition());
        tmp.max = toc2v(aabb.getPosition() + aabb.getSize());
        return tmp;
}

c2Circle shapeToc2(const Circle& circle)
{
        c2Circle tmp;
        tmp.p = toc2v(circle.getPosition());
        tmp.r = circle.getRadius();
        return tmp;
}

c2v toc2v(const sf::Vector2f& v)
{
        return c2V(v.x, v.y);
}
