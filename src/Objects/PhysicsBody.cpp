#include "PhysicsBody.h"

#define RADIAN_IN_DEGREES	57.3f
#define PIXEL_TO_METER      100.0f

PhysicsBody::~PhysicsBody()
{
	DeletePhysicsBody();
}

void PhysicsBody::InitPhysicsBody(std::vector<Node*>& Nodes, b2World* PhysicsWorld, b2BodyType BodyType, float Friction, float Density)
{
	b2BodyDef body_def;
	body_def.type = BodyType;
	body_def.angle = Angle / RADIAN_IN_DEGREES;
	body_def.position.x = Position.x / PIXEL_TO_METER;
	body_def.position.y = Position.y / PIXEL_TO_METER;
	m_Body = PhysicsWorld->CreateBody(&body_def);

	m_Density = Density;
	m_Friction = Friction;

	for (Node* node : Nodes)
		for (int child_id : m_ChildIDs)
			if (child_id == node->ID && node->NodeType == Node::Type::COLLIDER)
				SetCollider((Collider*)node);


	//b2Filter f;
	//f.maskBits = 
	//m_Body->GetFixtureList()[0].SetFilterData();
}

void PhysicsBody::SetCollider(Collider* _Collider)
{
	b2FixtureDef fixture_definition;

	switch (_Collider->ColliderType)
	{
	case Collider::Type::BOX:
		fixture_definition.shape = (b2PolygonShape*)_Collider->Shape;
		break;
	case Collider::Type::CIRCLE:
		fixture_definition.shape = (b2CircleShape*)_Collider->Shape;
		break;
	case Collider::Type::POLYGON:
		fixture_definition.shape = (b2PolygonShape*)_Collider->Shape;
		break;
	default:
		break;
	}
	
	fixture_definition.density = m_Density;
	fixture_definition.friction = m_Friction;

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