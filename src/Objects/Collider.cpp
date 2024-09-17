#include "Collider.h"
#include <box2d/box2d.h>

#define RADIAN_IN_DEGREES	57.3f
#define PIXEL_TO_METER      100.0f

Collider::~Collider()
{

}

void Collider::CreateBoxShape(Vector2 ColliderSize)
{
    PolygonShape = b2MakeBox(ColliderSize.x * Size.x, ColliderSize.y * Size.y);
    
    ColliderType = Type::BOX;
}

void Collider::CreateCircleShape(float Radius)
{
    CircleShape.center = LocalPosition;
    CircleShape.radius = Radius;

    ColliderType = Type::CIRCLE;
}

void Collider::CreatePolygonShape(Vector2 Polygons[], int32_t PolygonCount)
{
    b2Vec2 points[PolygonCount];

    for (int i = 0; i < PolygonCount; i++)
        points[i] = Polygons[i];

    b2Hull h = b2ComputeHull(points, PolygonCount);

    PolygonShape = b2MakePolygon(&h, 0.0f);

    ColliderType = Type::POLYGON;
}
