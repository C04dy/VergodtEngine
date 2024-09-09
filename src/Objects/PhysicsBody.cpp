#include "PhysicsBody.h"

#define RADIAN_IN_DEGREES	57.3f
#define PIXEL_TO_METER      100.0f

void PhysicsBody::InitPhysicsBodyBox(b2World* PhysicsWorld, b2BodyType BodyType, Vector2 ColliderSize, float Friction, float Density)
{
	b2BodyDef body_def;
	body_def.type = BodyType;
	body_def.angle = Angle / RADIAN_IN_DEGREES;
	body_def.position.x = Position.x / PIXEL_TO_METER;
	body_def.position.y = Position.y / PIXEL_TO_METER;
	m_Body = PhysicsWorld->CreateBody(&body_def);

	b2PolygonShape shape;

	shape.SetAsBox(((ColliderSize.x / 2) / PIXEL_TO_METER) * Size.x, ((ColliderSize.y / 2) / PIXEL_TO_METER) * Size.y);
	
	b2FixtureDef fixture_definition;
	fixture_definition.shape = &shape;
	fixture_definition.density = Density;
	fixture_definition.friction = Friction;

	m_Body->CreateFixture(&fixture_definition);

	//b2Filter f;
	//f.maskBits = 
	//m_Body->GetFixtureList()[0].SetFilterData();
}

void PhysicsBody::InitPhysicsBodyCircle(b2World* PhysicsWorld, b2BodyType BodyType, float Radius, float Friction, float Density)
{
	b2BodyDef body_def;
	body_def.type = BodyType;
	body_def.angle = Angle / RADIAN_IN_DEGREES;
	body_def.position.x = Position.x / PIXEL_TO_METER;
	body_def.position.y = Position.y / PIXEL_TO_METER;
	m_Body = PhysicsWorld->CreateBody(&body_def);

	b2CircleShape shape;
	shape.m_radius = Radius / PIXEL_TO_METER;
	
	b2FixtureDef fixture_definition;
	fixture_definition.shape = &shape;
	fixture_definition.density = Density;
	fixture_definition.friction = Friction;

	m_Body->CreateFixture(&fixture_definition);
}

void PhysicsBody::InitPhysicsBodyPolygon(b2World* PhysicsWorld, b2BodyType BodyType, Vector2 Polygons[], int32 PolygonCount, float Friction, float Density)
{
	b2BodyDef body_def;
	body_def.type = BodyType;
	body_def.angle = Angle / RADIAN_IN_DEGREES;
	body_def.position.x = Position.x / PIXEL_TO_METER;
	body_def.position.y = Position.y / PIXEL_TO_METER;
	m_Body = PhysicsWorld->CreateBody(&body_def);

	b2PolygonShape shape;

	b2Vec2 polygon_points[PolygonCount];

	for(int i = 0; i < PolygonCount; i++){
		polygon_points[i] = b2Vec2(Polygons[i].x / PIXEL_TO_METER, Polygons[i].y / PIXEL_TO_METER);
	}

	shape.Set(polygon_points, PolygonCount);

	b2FixtureDef fixture_definition;
	fixture_definition.shape = &shape;
	fixture_definition.density = Density;
	fixture_definition.friction = Friction;

	m_Body->CreateFixture(&fixture_definition);
}

void PhysicsBody::UpdatePhysicsNode()
{
	Position.x = m_Body->GetPosition().x * PIXEL_TO_METER;
	Position.y = m_Body->GetPosition().y * PIXEL_TO_METER;

	Angle = m_Body->GetAngle() * RADIAN_IN_DEGREES;
}

void PhysicsBody::DeletePhysicsBody()
{
	m_Body->GetWorld()->DestroyBody(m_Body);
	m_Body = nullptr;
}