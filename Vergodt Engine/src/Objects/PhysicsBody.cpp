#include "PhysicsBody.h"

void PhysicsBody::InitPhysicsBody(b2World& w, b2FixtureDef* f, b2BodyType bt)
{
	m_physicsbodydef.type = bt;
	m_physicsbodydef.position.Set(Transform.Position.getX(), Transform.Position.getY());
	m_physicsbody = w.CreateBody(&m_physicsbodydef);
	m_physicsbody->CreateFixture(f);
}

void PhysicsBody::UpdatePhysicsNode()
{
	Transform.Position.setX(m_physicsbody->GetPosition().x);
	Transform.Position.setY(-m_physicsbody->GetPosition().y);

	Transform.Angle = m_physicsbody->GetAngle();
}