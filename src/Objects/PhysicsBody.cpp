#include "PhysicsBody.h"

#define RADIAN_IN_DEGREES	57.3f

void PhysicsBody::InitPhysicsBodyBox(b2World* w, b2BodyType bt, float ColSizeX, float ColSizeY)
{
	m_physicsbodydef.type = bt;
	m_physicsbodydef.angle = Transform.Angle / RADIAN_IN_DEGREES;
	m_physicsbodydef.position = Transform.Position;
	m_physicsbody = w->CreateBody(&m_physicsbodydef);

	m_collisionshapePolygon = new b2PolygonShape;

	m_collisionshapePolygon->SetAsBox((ColSizeX / 2) * Transform.Size.x, (ColSizeY / 2) * Transform.Size.y);
	b2FixtureDef m_fixturedef;
	m_fixturedef.shape = m_collisionshapePolygon;
	m_fixturedef.density = 1.0f;
	m_fixturedef.friction = 0.3f;

	if(bt == b2BodyType::b2_dynamicBody){
		m_physicsbody->CreateFixture(m_collisionshapePolygon, 1.0f);
	}else{
		m_physicsbody->CreateFixture(m_collisionshapePolygon, 0.0f);
	}
}

void PhysicsBody::InitPhysicsBodyCircle(b2World* w, b2BodyType bt, float Radius)
{
	m_physicsbodydef.type = bt;
	m_physicsbodydef.angle = Transform.Angle / RADIAN_IN_DEGREES;
	m_physicsbodydef.position = Transform.Position;
	m_physicsbody = w->CreateBody(&m_physicsbodydef);

	m_collisionshapecircle = new b2CircleShape;

	m_collisionshapecircle->m_radius = Radius;
	b2FixtureDef m_fixturedef;
	m_fixturedef.shape = m_collisionshapecircle;

	if(bt == b2BodyType::b2_dynamicBody){
		m_physicsbody->CreateFixture(m_collisionshapecircle, 1.0f);
	}else{
		m_physicsbody->CreateFixture(m_collisionshapecircle, 0.0f);
	}
}

void PhysicsBody::InitPhysicsBodyPolygon(b2World* w, b2BodyType bt, Vector2 Polygons[], int32 PolygonCount)
{
	m_physicsbodydef.type = bt;
	m_physicsbodydef.angle = Transform.Angle / RADIAN_IN_DEGREES;
	m_physicsbodydef.position = Transform.Position;
	m_physicsbody = w->CreateBody(&m_physicsbodydef);

	m_collisionshapePolygon = new b2PolygonShape;

	m_collisionshapePolygon->Set(Polygons, PolygonCount);

	if(bt == b2BodyType::b2_dynamicBody){
		m_physicsbody->CreateFixture(m_collisionshapePolygon, 1.0f);
	}else{
		m_physicsbody->CreateFixture(m_collisionshapePolygon, 0.0f);
	}
}


void PhysicsBody::UpdatePhysicsNode()
{
	Transform.Position = m_physicsbody->GetPosition();
	Transform.Angle = m_physicsbody->GetAngle() * RADIAN_IN_DEGREES;
}