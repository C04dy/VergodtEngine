#include "PhysicsBody.h"

#define RADIAN_IN_DEGREES	57.3f

void PhysicsBody::InitPhysicsBodyBox(b2World* PhysicsWorld, b2BodyType BodyType, float ColSizeX, float ColSizeY){
	m_physicsbodydef.type = BodyType;
	m_physicsbodydef.angle = Transform.Angle / RADIAN_IN_DEGREES;
	m_physicsbodydef.position.x = Transform.Position.x / 48;
	m_physicsbodydef.position.y = Transform.Position.y / 48;
	m_physicsbody = PhysicsWorld->CreateBody(&m_physicsbodydef);

	m_collisionshapePolygon = new b2PolygonShape;

	m_collisionshapePolygon->SetAsBox(((ColSizeX / 2) / 48) * Transform.Size.x, ((ColSizeY / 2) / 48) * Transform.Size.y);
	b2FixtureDef m_fixturedef;
	m_fixturedef.shape = m_collisionshapePolygon;
	m_fixturedef.density = 1.0f;
	m_fixturedef.friction = 0.3f;

	if(BodyType == b2BodyType::b2_dynamicBody){
		m_physicsbody->CreateFixture(m_collisionshapePolygon, 1.0f);
	}else{
		m_physicsbody->CreateFixture(m_collisionshapePolygon, 0.0f);
	}
}

void PhysicsBody::InitPhysicsBodyCircle(b2World* PhysicsWorld, b2BodyType BodyType, float Radius){
	m_physicsbodydef.type = BodyType;
	m_physicsbodydef.angle = Transform.Angle / RADIAN_IN_DEGREES;
	m_physicsbodydef.position.x = Transform.Position.x / 48;
	m_physicsbodydef.position.y = Transform.Position.y / 48;
	m_physicsbody = PhysicsWorld->CreateBody(&m_physicsbodydef);

	m_collisionshapecircle = new b2CircleShape;

	m_collisionshapecircle->m_radius = Radius / 48;
	b2FixtureDef m_fixturedef;
	m_fixturedef.shape = m_collisionshapecircle;

	if(BodyType == b2BodyType::b2_dynamicBody){
		m_physicsbody->CreateFixture(m_collisionshapecircle, 1.0f);
	}else{
		m_physicsbody->CreateFixture(m_collisionshapecircle, 0.0f);
	}
}

void PhysicsBody::InitPhysicsBodyPolygon(b2World* PhysicsWorld, b2BodyType BodyType, Vector2 Polygons[], int32 PolygonCount){
	m_physicsbodydef.type = BodyType;
	m_physicsbodydef.angle = Transform.Angle / RADIAN_IN_DEGREES;
	m_physicsbodydef.position.x = Transform.Position.x / 48;
	m_physicsbodydef.position.y = Transform.Position.y / 48;
	m_physicsbody = PhysicsWorld->CreateBody(&m_physicsbodydef);

	m_collisionshapePolygon = new b2PolygonShape;

	for(int i = 0; i < PolygonCount; i++){
		Polygons[i].x = Polygons[i].x / 48;
		Polygons[i].y = Polygons[i].y / 48;
	}
	
	m_collisionshapePolygon->Set(Polygons, PolygonCount);

	if(BodyType == b2BodyType::b2_dynamicBody){
		m_physicsbody->CreateFixture(m_collisionshapePolygon, 1.0f);
	}else{
		m_physicsbody->CreateFixture(m_collisionshapePolygon, 0.0f);
	}
}

void PhysicsBody::UpdatePhysicsNode(){
	Transform.Position.x = m_physicsbody->GetPosition().x * 48;
	Transform.Position.y = m_physicsbody->GetPosition().y * 48;
	Transform.Angle = m_physicsbody->GetAngle() * RADIAN_IN_DEGREES;
}

void PhysicsBody::CleanPhysicsNode(){
	m_physicsbody = nullptr;
	m_collisionshapePolygon = nullptr;
	m_collisionshapecircle = nullptr;
}