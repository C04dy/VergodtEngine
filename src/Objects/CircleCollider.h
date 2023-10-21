#pragma once
#include "Collider.h"

class CircleCollider : public Collider
{
public:
	void InitCircleCollider();
	b2Shape* GetCollider() { return &m_collisionshapecircle; }
	b2FixtureDef* GetFixture() { return &m_fixturedef; }
	float Radius;
private:
	b2CircleShape m_collisionshapecircle;
	b2FixtureDef m_fixturedef;
};

