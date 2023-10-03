#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
public:
	void InitBoxCollider();
	b2Shape* GetCollider() { return &m_collisionshapebox; }
	b2FixtureDef* GetFixture() { return &m_fixturedef; }
	Vector2 ColliderSize;
private:
	b2PolygonShape m_collisionshapebox;
	b2FixtureDef m_fixturedef;
};

