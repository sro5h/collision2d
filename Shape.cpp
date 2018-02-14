#include "Shape.hpp"
#define TINYC2_IMPLEMENTATION
#include "tinyc2.h"

void handleResult(const c2Manifold&, Manifold&);
void handleResult(const c2Raycast&, const c2Ray&, Raycast&);
c2AABB shapeToc2(const Aabb&, const sf::Vector2f);
c2Circle shapeToc2(const Circle&, const sf::Vector2f);
c2Ray rayToc2(const Ray&, const sf::Vector2f);
c2v toc2v(const sf::Vector2f&);
sf::Vector2f tosfv(const c2v&);

Shape::Shape(Type type)
        : mType(type)
{
}

Type Shape::getType() const
{
        return mType;
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

Ray::Ray(sf::Vector2f direction, float length)
        : mDirection(direction)
        , mLength(length)
{
}

Ray::Ray(float x, float y, float length)
        : mDirection(x, y)
        , mLength(length)
{
}

sf::Vector2f Ray::getDirection() const
{
        return mDirection;
}

void Ray::setDirection(sf::Vector2f direction)
{
        mDirection = direction;
}

void Ray::setDirection(float x, float y)
{
        mDirection.x = x;
        mDirection.y = y;
}

float Ray::getLength() const
{
        return mLength;
}

void Ray::setLength(float length)
{
        mLength = length;
}

Manifold::Manifold()
        : colliding(false)
        , normal(0, 0)
        , contact(0, 0)
        , depth(0)
{
}

void Manifold::solve(const Shape& a, const sf::Vector2f posA,
                const Shape& b, const sf::Vector2f posB)
{
        mPositionA = posA;
        mPositionB = posB;

        dispatch(a, b);
}

void Manifold::dispatch(const Shape& a, const Shape& b)
{
        if (a.getType() == Type::Aabb)
        {
                const Aabb& aChild = castShape<Aabb>(a);

                if (b.getType() == Type::Aabb)
                {
                        const Aabb& bChild = castShape<Aabb>(b);
                        collide(aChild, bChild);
                }
                else if (b.getType() == Type::Circle)
                {
                        const Circle& bChild = castShape<Circle>(b);
                        collide(aChild, bChild);
                }
        }
        else if (a.getType() == Type::Circle)
        {
                const Circle& aChild = castShape<Circle>(a);

                if (b.getType() == Type::Aabb)
                {
                        const Aabb& bChild = castShape<Aabb>(b);
                        collide(aChild, bChild);
                }
                else if (b.getType() == Type::Circle)
                {
                        const Circle& bChild = castShape<Circle>(b);
                        collide(aChild, bChild);
                }
        }
}

void Manifold::collide(const Aabb& a, const Aabb& b)
{
        c2AABB shapeA = shapeToc2(a, mPositionA);
        c2AABB shapeB = shapeToc2(b, mPositionB);
        c2Manifold m;
        c2AABBtoAABBManifold(shapeA, shapeB, &m);

        handleResult(m, *this);
}

void Manifold::collide(const Circle& a, const Circle& b)
{
        c2Circle shapeA = shapeToc2(a, mPositionA);
        c2Circle shapeB = shapeToc2(b, mPositionB);
        c2Manifold m;
        c2CircletoCircleManifold(shapeA, shapeB, &m);

        handleResult(m, *this);
}

void Manifold::collide(const Aabb& a, const Circle& b)
{
        collide(b, a);
        normal = -normal;
}

void Manifold::collide(const Circle& a, const Aabb& b)
{
        c2Circle shapeA = shapeToc2(a, mPositionA);
        c2AABB shapeB = shapeToc2(b, mPositionB);
        c2Manifold m;
        c2CircletoAABBManifold(shapeA, shapeB, &m);

        handleResult(m, *this);
}

Raycast::Raycast()
        : hit(false)
        , normal(0, 0)
        , t(0)
{
}

void Raycast::solve(const Ray& a, const sf::Vector2f posA,
                const Shape& b, const sf::Vector2f posB)
{
        mPositionA = posA;
        mPositionB = posB;

        dispatch(a, b);
}

void Raycast::dispatch(const Ray& a, const Shape& b)
{
        if (b.getType() == Type::Aabb)
        {
                const Aabb& bChild = castShape<Aabb>(b);
                calculate(a, bChild);
        }
        else if (b.getType() == Type::Circle)
        {
                const Circle& bChild = castShape<Circle>(b);
                calculate(a, bChild);
        }
}

void Raycast::calculate(const Ray& a, const Aabb& b)
{
        c2Ray ray = rayToc2(a, mPositionA);
        c2AABB shape = shapeToc2(b, mPositionB);
        c2Raycast r;
        hit = false;

        if (c2RaytoAABB(ray, shape, &r))
                handleResult(r, ray, *this);
}

void Raycast::calculate(const Ray& a, const Circle& b)
{
        c2Ray ray = rayToc2(a, mPositionA);
        c2Circle shape = shapeToc2(b, mPositionB);
        c2Raycast r;
        hit = false;

        if (c2RaytoCircle(ray, shape, &r))
                handleResult(r, ray, *this);
}

void handleResult(const c2Manifold& manifold, Manifold& m)
{
        m.colliding = manifold.count > 0;
        if (m.colliding)
        {
                m.normal = tosfv(manifold.normal);
                m.contact = tosfv(manifold.contact_points[0]);
                m.depth = manifold.depths[0];
        }
}

void handleResult(const c2Raycast& raycast, const c2Ray& ray, Raycast& r)
{
        r.hit = true;
        r.normal = tosfv(raycast.n);
        r.contact = tosfv(c2Impact(ray, raycast.t));
        r.t = raycast.t;
}

c2AABB shapeToc2(const Aabb& aabb, const sf::Vector2f position)
{
        c2AABB tmp;
        tmp.min = toc2v(position);
        tmp.max = toc2v(position + aabb.getSize());
        return tmp;
}

c2Circle shapeToc2(const Circle& circle, const sf::Vector2f position)
{
        c2Circle tmp;
        tmp.p = toc2v(position);
        tmp.r = circle.getRadius();
        return tmp;
}

c2Ray rayToc2(const Ray& ray, const sf::Vector2f position)
{
        c2Ray tmp;
        tmp.p = toc2v(position);
        tmp.d = toc2v(ray.getDirection());
        tmp.t = ray.getLength();
        return tmp;
}

c2v toc2v(const sf::Vector2f& v)
{
        return c2V(v.x, v.y);
}

sf::Vector2f tosfv(const c2v& v)
{
        return sf::Vector2f(v.x, v.y);
}
