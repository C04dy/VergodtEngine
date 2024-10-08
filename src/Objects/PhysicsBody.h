#pragma once
#include "Node.h"
#include "Collider.h"
#include <box2d/box2d.h>

class PhysicsBody : public Node
{
public:
	~PhysicsBody();
	
	void InitPhysicsBody(std::vector<Node*>& Nodes, b2WorldId& PhysicsWorldID, b2BodyType BodyType, float Friction = 0.3f, float Density = 1.0f);

	void SetCollider(Collider* _Collider);

	void DeletePhysicsBody();

	void UpdatePhysicsNode();

	b2BodyId BodyID;
private:

	float m_Friction = 0.0f, m_Density = 0.0f;
};