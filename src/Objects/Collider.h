#ifndef COLLIDER_H
#define COLLIDER_H

#pragma once

#include "Node.h"

class b2Shape;

class Collider : public Node
{
public:
    enum class Type
    {
        NONE = 0,
        BOX = 1,
        CIRCLE = 2,
        POLYGON = 3
    };
public:
    ~Collider();

    void CreateBoxShape(Vector2 ColliderSize);

    void CreateCircleShape(float Radius);

    void CreatePolygonShape(Vector2 Polygons[], int32 PolygonCount);
public:
    Type ColliderType = Type::NONE;

    b2Shape* Shape = nullptr;
};

#endif