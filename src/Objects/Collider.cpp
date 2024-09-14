#include "Collider.h"
#include <box2d/box2d.h>

#define RADIAN_IN_DEGREES	57.3f
#define PIXEL_TO_METER      100.0f

Collider::~Collider()
{
    delete Shape;
    Shape = nullptr;
}

void Collider::CreateBoxShape(Vector2 ColliderSize)
{
    Shape = new b2PolygonShape();

    ((b2PolygonShape*)Shape)->SetAsBox(((ColliderSize.x / 2) / PIXEL_TO_METER) * Size.x, ((ColliderSize.y / 2) / PIXEL_TO_METER) * Size.y);

    ColliderType = Type::BOX;
}

void Collider::CreateCircleShape(float Radius)
{
    Shape = new b2CircleShape();

    ((b2CircleShape*)Shape)->m_radius = Radius / PIXEL_TO_METER;

    ColliderType = Type::CIRCLE;
}

void Collider::CreatePolygonShape(Vector2 Polygons[], int32 PolygonCount)
{
    Shape = new b2PolygonShape();

	b2Vec2 polygon_points[PolygonCount];

	for (int i = 0; i < PolygonCount; i++)
		polygon_points[i] = b2Vec2(Polygons[i].x / PIXEL_TO_METER, Polygons[i].y / PIXEL_TO_METER);

    ((b2PolygonShape*)Shape)->Set(polygon_points, PolygonCount);

    ColliderType = Type::POLYGON;
}
