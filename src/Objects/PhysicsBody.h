#pragma once
#include "Node.h"
#include <box2d/box2d.h>

class Body {
public:
	void SetBody(b2Body* b) { m_physicsbody = b; }

	void SetFixedRotation(bool flag) { m_physicsbody->SetFixedRotation(flag); }
	void SetVelocity(Vector2 v) { m_physicsbody->SetLinearVelocity(v); }
	Vector2 GetVelocity() { return Vector2(m_physicsbody->GetLinearVelocity().x,m_physicsbody->GetLinearVelocity().y); }
private:
	b2Body* m_physicsbody;
};

class PhysicsBody : public Node
{
public:
	void InitPhysicsBodyBox(b2World* PhysicsWorld, b2BodyType BodyType, Vector2 ColSize, float Friction = 0.3f, float Density = 1.0f);
	void InitPhysicsBodyCircle(b2World* PhysicsWorld, b2BodyType BodyType, float Radius, float Friction = 0.3f, float Density = 1.0f);
	void InitPhysicsBodyPolygon(b2World* PhysicsWorld, b2BodyType BodyType, Vector2 Polygons[], int32 PolygonCount, float Friction = 0.3f, float Density = 1.0f);

	void DeletePhysicsBody();

	b2Body* GetBody() { return m_physicsbody; }

	Body body;

	void UpdatePhysicsNode();
private:
	b2Body* m_physicsbody;
};