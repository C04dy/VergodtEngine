#ifndef POLYGONCOLLIDER_H
#define POLYGONCOLLIDER_H

#pragma once

#include "Collider.h"

class PolygonCollider : public Collider
{
public:
    void InitPolygonCollider(Vector2 Polygons[], int32 PolygonCount);
	b2Shape* GetCollider() { return &m_collisionshape; }
private:
    b2PolygonShape m_collisionshape;
};

#endif