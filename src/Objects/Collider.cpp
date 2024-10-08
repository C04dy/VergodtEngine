#include "Collider.h"
#include <box2d/box2d.h>

#define PIXEL_TO_METER      100.0f

Collider::~Collider()
{

}

void Collider::CreateBoxShape(Vector2 ColliderSize)
{
    BoxColliderSize = { ColliderSize.x * Size.x / PIXEL_TO_METER / 2, ColliderSize.y * Size.y / PIXEL_TO_METER / 2 };
    
    ColliderType = Type::BOX;
}

void Collider::CreateCircleShape(float Radius)
{
    CircleRadius = Radius / PIXEL_TO_METER;

    ColliderType = Type::CIRCLE;
}

void Collider::CreatePolygonShape(Vector2 Polygons[], int32_t PolygonCount)
{
    b2Vec2 points[PolygonCount];

    for (int i = 0; i < PolygonCount; i++)
        points[i] = { -Polygons[i].x / PIXEL_TO_METER, -Polygons[i].y / PIXEL_TO_METER };

    std::cout << PolygonCount;
    PolygonHull = b2ComputeHull(points, PolygonCount);

    if (PolygonHull.count == 0)
        std::cout << "Couldnt create polygon shape";

    ColliderType = Type::POLYGON;
}
