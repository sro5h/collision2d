collision2d
===========
A small wrapper around [tinyc2][1] using [SFML][2]'s `sf::Vector2f` (for now).

Features
--------
* For now circles, aabbs and rays are the only supported shape types
* Manifolds and raycasts can be calculated for all supported shapes

How to use
----------
If you are already using SFML and tinyc2, it should be enough to add
`Collision.hpp` and `Collision.cpp` to your project.  
Then create your shapes
```c++
std::unique_ptr<Shape> shapeA = std::make_unique<Circle>(30.0f);
std::unique_ptr<Shape> shapeB = std::make_unique<Aabb>(60.0f, 80.0f);
```
Calculate a manifold
```c++
Manifold m;
m = solve(*shapeA, sf::Vector2f(300.0f, 200.0f), *shapeB, sf::Vector2f(250.0f, 250.0f));
```
And solve the collision
```c++
if (m.colliding)
{
        positionA -= m.normal * m.depth;
}
```

License
-------
collision2d uses the same license as [tinyheaders][1], the zlib license.

[1]: https://github.com/randygaul/tinyheaders
[2]: https://github.com/sfml/sfml
