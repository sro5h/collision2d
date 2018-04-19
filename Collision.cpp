#include "Collision.hpp"
#define TINYC2_IMPLEMENTATION
#include "tinyc2.h"

namespace priv
{
template <typename T>
struct Helper
{
        Helper(const T& shape, sf::Vector2f position);

        const T& shape;
        sf::Vector2f position;
};

Manifold calculate(Helper<Aabb> a, Helper<Aabb> b);
Manifold calculate(Helper<Circle> a, Helper<Circle> b);
Manifold calculate(Helper<Aabb> a, Helper<Circle> b);
Manifold calculate(Helper<Circle> a, Helper<Aabb> b);
Raycast calculate(Helper<Ray> a, Helper<Aabb> b);
Raycast calculate(Helper<Ray> a, Helper<Circle> b);

Manifold toManifold(const c2Manifold& m);
Raycast toRaycast(const c2Raycast& r, const c2Ray& ray);

c2AABB toc2AABB(Helper<Aabb> aabb);
c2Circle toc2Circle(Helper<Circle> circle);
c2Ray toc2Ray(Helper<Ray> ray);
c2v toc2v(const sf::Vector2f& v);
sf::Vector2f tosfv(const c2v& v);
}

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

Raycast::Raycast()
        : hit(false)
        , normal(0, 0)
        , contact(0, 0)
        , t(0)
{
}

template <typename T>
priv::Helper<T>::Helper(const T& shape, sf::Vector2f position)
        : shape(shape)
        , position(position)
{
}

Manifold solve(const Shape& a, sf::Vector2f posA, const Shape& b,
               sf::Vector2f posB)
{
        Manifold m;

        if (a.getType() == Type::Aabb)
        {
                priv::Helper<Aabb> ha(castShape<Aabb>(a), posA);

                if (b.getType() == Type::Aabb)
                {
                        priv::Helper<Aabb> hb(castShape<Aabb>(b), posB);
                        m = priv::calculate(ha, hb);
                }
                else if (b.getType() == Type::Circle)
                {
                        priv::Helper<Circle> hb(castShape<Circle>(b), posB);
                        m = priv::calculate(ha, hb);
                }
        }
        else if (a.getType() == Type::Circle)
        {
                priv::Helper<Circle> ha(castShape<Circle>(a), posA);

                // Could use a function castOther<T>() to avoid duplicating this
                if (b.getType() == Type::Aabb)
                {
                        priv::Helper<Aabb> hb(castShape<Aabb>(b), posB);
                        m = priv::calculate(ha, hb);
                }
                else if (b.getType() == Type::Circle)
                {
                        priv::Helper<Circle> hb(castShape<Circle>(b), posB);
                        m = priv::calculate(ha, hb);
                }
        }

        return m;
}

Raycast solve(const Ray& a, sf::Vector2f posA, const Shape& b,
               sf::Vector2f posB)
{
        Raycast r;
        priv::Helper<Ray> ha(a, posA);

        if (b.getType() == Type::Aabb)
        {
                priv::Helper<Aabb> hb(castShape<Aabb>(b), posB);
                r = priv::calculate(ha, hb);
        }
        else if (b.getType() == Type::Circle)
        {
                priv::Helper<Circle> hb(castShape<Circle>(b), posB);
                r = priv::calculate(ha, hb);
        }

        return r;
}

Manifold priv::calculate(Helper<Aabb> a, Helper<Aabb> b)
{
        c2AABB shapeA = priv::toc2AABB(a);
        c2AABB shapeB = priv::toc2AABB(b);
        c2Manifold m;
        c2AABBtoAABBManifold(shapeA, shapeB, &m);

        return priv::toManifold(m);
}

Manifold priv::calculate(Helper<Circle> a, Helper<Circle> b)
{
        c2Circle shapeA = priv::toc2Circle(a);
        c2Circle shapeB = priv::toc2Circle(b);
        c2Manifold m;
        c2CircletoCircleManifold(shapeA, shapeB, &m);

        return priv::toManifold(m);
}

Manifold priv::calculate(Helper<Aabb> a, Helper<Circle> b)
{
        Manifold m = priv::calculate(b, a);
        m.normal = -m.normal;

        return m;
}

Manifold priv::calculate(Helper<Circle> a, Helper<Aabb> b)
{
        c2Circle shapeA = priv::toc2Circle(a);
        c2AABB shapeB = priv::toc2AABB(b);
        c2Manifold m;
        c2CircletoAABBManifold(shapeA, shapeB, &m);

        return priv::toManifold(m);
}

Raycast priv::calculate(Helper<Ray> a, Helper<Aabb> b)
{
        Raycast tmp;
        c2Ray ray = priv::toc2Ray(a);
        c2AABB shape = priv::toc2AABB(b);
        c2Raycast r;

        if (c2RaytoAABB(ray, shape, &r))
                tmp = priv::toRaycast(r, ray);

        return tmp;
}

Raycast priv::calculate(Helper<Ray> a, Helper<Circle> b)
{
        Raycast tmp;
        c2Ray ray = priv::toc2Ray(a);
        c2Circle shape = priv::toc2Circle(b);
        c2Raycast r;

        if (c2RaytoCircle(ray, shape, &r))
                tmp = priv::toRaycast(r, ray);

        return tmp;
}

Manifold priv::toManifold(const c2Manifold& m)
{
        Manifold tmp;
        tmp.colliding = m.count > 0;

        if (tmp.colliding)
        {
                tmp.normal = tosfv(m.normal);
                tmp.contact = tosfv(m.contact_points[0]);
                tmp.depth = m.depths[0];
        }

        return tmp;
}

Raycast priv::toRaycast(const c2Raycast& r, const c2Ray& ray)
{
        Raycast tmp;
        tmp.hit = true;
        tmp.normal = tosfv(r.n);
        tmp.contact = tosfv(c2Impact(ray, r.t));
        tmp.t = r.t;

        return tmp;
}

c2AABB priv::toc2AABB(Helper<Aabb> aabb)
{
        c2AABB tmp;
        tmp.min = toc2v(aabb.position);
        tmp.max = toc2v(aabb.position + aabb.shape.getSize());
        return tmp;
}

c2Circle priv::toc2Circle(Helper<Circle> circle)
{
        c2Circle tmp;
        tmp.p = toc2v(circle.position);
        tmp.r = circle.shape.getRadius();
        return tmp;
}

c2Ray priv::toc2Ray(Helper<Ray> ray)
{
        c2Ray tmp;
        tmp.p = toc2v(ray.position);
        tmp.d = toc2v(ray.shape.getDirection());
        tmp.t = ray.shape.getLength();
        return tmp;
}

c2v priv::toc2v(const sf::Vector2f& v)
{
        return c2V(v.x, v.y);
}

sf::Vector2f priv::tosfv(const c2v& v)
{
        return sf::Vector2f(v.x, v.y);
}
