#include "PhysicsBody.h"

#define PIXEL_TO_METER      100.0f

PhysicsBody::~PhysicsBody()
{
	DeletePhysicsBody();
}

void PhysicsBody::InitPhysicsBody(std::vector<Node*>& Nodes, b2WorldId& PhysicsWorldID, b2BodyType BodyType, float Friction, float Density)
{
	b2BodyDef body_def = b2DefaultBodyDef();
	body_def.type = BodyType;
	body_def.rotation = b2MakeRot(Angle);
	body_def.position.x = Position.x / PIXEL_TO_METER;
	body_def.position.y = Position.y / PIXEL_TO_METER;
	BodyID = b2CreateBody(PhysicsWorldID, &body_def);

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
	b2ShapeDef shape_definition = b2DefaultShapeDef();

	shape_definition.density = m_Density;
	shape_definition.friction = m_Friction;

	switch (_Collider->ColliderType)
	{
	case Collider::Type::BOX:
		b2CreatePolygonShape(BodyID, &shape_definition, &_Collider->PolygonShape);
		break;
	case Collider::Type::CIRCLE:
		b2CreateCircleShape(BodyID, &shape_definition, &_Collider->CircleShape);
		break;
	case Collider::Type::POLYGON:
		b2CreatePolygonShape(BodyID, &shape_definition, &_Collider->PolygonShape);
		break;
	default:
		break;
	}
}

void PhysicsBody::UpdatePhysicsNode()
{
	b2Body_GetPosition(BodyID);
	Position = b2Body_GetPosition(BodyID) * PIXEL_TO_METER;

	Angle = std::asin(b2Body_GetRotation(BodyID).s) * (180.0 / M_PI);
}

void PhysicsBody::DeletePhysicsBody()
{
	b2DestroyBody(BodyID);
}