#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
	void InitBoxCollider();
	b2Shape* GetCollider() { return m_collisionshapebox; }
	Vector2 ColliderSize;
private:
	b2PolygonShape* m_collisionshapebox;
};

