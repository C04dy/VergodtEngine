#pragma once
#include "Node.h"
#include <box2d/box2d.h>

class Collider : public Node
{
public:
	virtual b2Shape* GetCollider() { return nullptr; }
	b2FixtureDef* GetFixture() { return &m_fixturedef; }
protected:
	b2FixtureDef m_fixturedef;
};

