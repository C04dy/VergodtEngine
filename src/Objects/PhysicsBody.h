#pragma once
#include "Node.h"
#include <box2d/box2d.h>

class PhysicsBody : public Node
{
public:
	PhysicsBody(const std::string& name = "") : Node::Node(name) {};

	void InitPhysicsBodyBox(b2World* PhysicsWorld, b2BodyType BodyType, Vector2 ColSize, float Friction = 0.3f, float Density = 1.0f);
	void InitPhysicsBodyCircle(b2World* PhysicsWorld, b2BodyType BodyType, float Radius, float Friction = 0.3f, float Density = 1.0f);
	void InitPhysicsBodyPolygon(b2World* PhysicsWorld, b2BodyType BodyType, Vector2 Polygons[], int32 PolygonCount, float Friction = 0.3f, float Density = 1.0f);

	void DeletePhysicsBody();

	b2Body* GetBody() { return m_physicsbody; }

	void SetFixedRotation(bool flag) { m_physicsbody->SetFixedRotation(flag); }

	void UpdatePhysicsNode();
private:
	b2Body* m_physicsbody;
};