#include "PhysicsBody.h"

void PhysicsBody::InitPhysicsBody(b2World* w, Collider* col, b2BodyType bt)
{
	m_physicsbodydef.type = bt;
	m_physicsbodydef.position = Transform.Position;
	m_physicsbody = w->CreateBody(&m_physicsbodydef);
	if(bt == b2BodyType::b2_dynamicBody){
		m_physicsbody->CreateFixture(col->GetFixture());
	}else{
		m_physicsbody->CreateFixture(col->GetCollider(), 0.0f);
	}
}

void PhysicsBody::UpdatePhysicsNode()
{
	Transform.Position = m_physicsbody->GetPosition();

	Transform.Angle = m_physicsbody->GetAngle();
}