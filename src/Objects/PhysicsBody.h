#pragma once
#include "Node.h"
#include <box2d/box2d.h>

class PhysicsBody : public Node
{
public:
	void InitPhysicsBodyBox(b2World* w, b2BodyType bt, float ColSizeX, float ColSizeY);
	void InitPhysicsBodyCircle(b2World* w, b2BodyType bt, float Radius);
	void InitPhysicsBodyPolygon(b2World* w, b2BodyType bt, Vector2 Polygons[], int32 PolygonCount);

	b2Body* GetBody() { return m_physicsbody; }

	void UpdatePhysicsNode();
private:
	b2BodyDef m_physicsbodydef;
	b2Body* m_physicsbody;

	b2PolygonShape* m_collisionshapePolygon;
	b2CircleShape* m_collisionshapecircle;
};