#pragma once
#include "Node.h"
#include "Collider.h"
#include <box2d/b2_body.h>

class PhysicsBody : public Node
{
public:
	void InitPhysicsBody(b2World& w, b2FixtureDef* f, b2BodyType bt);

	b2Body* GetBody() { return m_physicsbody; }

	void UpdatePhysicsNode();
private:
	b2BodyDef m_physicsbodydef;
	b2Body* m_physicsbody;
	Collider* m_collider;
};