#include "PhysicsBody.h"

#define RADIAN_IN_DEGREES	57.3f
#define PIXEL_TO_METER      100.0f

void PhysicsBody::InitPhysicsBodyBox(b2World* PhysicsWorld, b2BodyType BodyType, Vector2 ColSize, float Friction, float Density){
	b2BodyDef physicsbodydef;
	physicsbodydef.type = BodyType;
	physicsbodydef.angle = Angle / RADIAN_IN_DEGREES;
	physicsbodydef.position.x = Position.x / PIXEL_TO_METER;
	physicsbodydef.position.y = Position.y / PIXEL_TO_METER;
	m_physicsbody = PhysicsWorld->CreateBody(&physicsbodydef);

	b2PolygonShape shape;

	shape.SetAsBox(((ColSize.x / 2) / PIXEL_TO_METER) * Size.x, ((ColSize.y / 2) / PIXEL_TO_METER) * Size.y);
	
	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = Density;
	fixturedef.friction = Friction;

	m_physicsbody->CreateFixture(&fixturedef);

	Type = NodeType::PHYSICSBODY;

	body.SetBody(m_physicsbody);
}

void PhysicsBody::InitPhysicsBodyCircle(b2World* PhysicsWorld, b2BodyType BodyType, float Radius, float Friction, float Density){
	b2BodyDef physicsbodydef;
	physicsbodydef.type = BodyType;
	physicsbodydef.angle = Angle / RADIAN_IN_DEGREES;
	physicsbodydef.position.x = Position.x / PIXEL_TO_METER;
	physicsbodydef.position.y = Position.y / PIXEL_TO_METER;
	m_physicsbody = PhysicsWorld->CreateBody(&physicsbodydef);

	b2CircleShape shape;
	shape.m_radius = Radius / PIXEL_TO_METER;
	
	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = Density;
	fixturedef.friction = Friction;

	m_physicsbody->CreateFixture(&fixturedef);

	Type = NodeType::PHYSICSBODY;

	body.SetBody(m_physicsbody);
}

void PhysicsBody::InitPhysicsBodyPolygon(b2World* PhysicsWorld, b2BodyType BodyType, Vector2 Polygons[], int32 PolygonCount, float Friction, float Density){
	b2BodyDef physicsbodydef;
	physicsbodydef.type = BodyType;
	physicsbodydef.angle = Angle / RADIAN_IN_DEGREES;
	physicsbodydef.position.x = Position.x / PIXEL_TO_METER;
	physicsbodydef.position.y = Position.y / PIXEL_TO_METER;
	m_physicsbody = PhysicsWorld->CreateBody(&physicsbodydef);

	b2PolygonShape shape;

	b2Vec2 polys[PolygonCount];

	for(int i = 0; i < PolygonCount; i++){
		polys[i] = b2Vec2(Polygons[i].x / PIXEL_TO_METER, Polygons[i].y / PIXEL_TO_METER);
	}

	shape.Set(polys, PolygonCount);

	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = Density;
	fixturedef.friction = Friction;

	m_physicsbody->CreateFixture(&fixturedef);

	Type = NodeType::PHYSICSBODY;

	body.SetBody(m_physicsbody);
}

void PhysicsBody::UpdatePhysicsNode(){
	Position.x = m_physicsbody->GetPosition().x * PIXEL_TO_METER;
	Position.y = m_physicsbody->GetPosition().y * PIXEL_TO_METER;

	Angle = m_physicsbody->GetAngle() * RADIAN_IN_DEGREES;
}

void PhysicsBody::DeletePhysicsBody(){
	m_physicsbody->GetWorld()->DestroyBody(m_physicsbody);
	m_physicsbody = nullptr;
}