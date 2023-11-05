#include "PolygonCollider.h"

void PolygonCollider::InitPolygonCollider(Vector2 Polygons[], int32 PolygonCount)
{
    m_collisionshape.Set(Polygons, PolygonCount);
}