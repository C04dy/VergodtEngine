#pragma once
#include "Node.h"
#include <box2d/box2d.h>

class Collider : public Node
{
public:
	virtual b2Shape* GetCollider() { return &b; }
private:
	b2PolygonShape b;
};

