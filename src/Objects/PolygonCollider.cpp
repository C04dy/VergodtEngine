#include "PolygonCollider.h"

void PolygonCollider::InitPolygonCollider(int32 PolygonCount)
{
    m_polygoncount = PolygonCount;
    m_collisionshape.Set(nullptr , m_polygoncount);
}