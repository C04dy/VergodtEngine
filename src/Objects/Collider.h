#pragma once
#include "Node.h"
#include <box2d/box2d.h>

class Collider : public Node
{
public:
	virtual b2Shape* GetCollider() { return nullptr; }
	virtual b2FixtureDef* GetFixture() { return nullptr; }
};

