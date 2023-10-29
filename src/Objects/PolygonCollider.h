#ifndef POLYGONCOLLIDER_H
#define POLYGONCOLLIDER_H

#pragma once

#include "Collider.h"

class PolygonCollider : public Collider
{
public:
    void InitPolygonCollider(int32 PolygonCount);
	b2Shape* GetCollider() { return &m_collisionshape; }
private:
    b2PolygonShape m_collisionshape;
    int32 m_polygoncount;
};

#endif