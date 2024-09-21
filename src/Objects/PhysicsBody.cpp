#include "PhysicsBody.h"

#define PIXEL_TO_METER      100.0f

PhysicsBody::~PhysicsBody()
{
	DeletePhysicsBody();
}

static float DegreeToRadian(float Degree)
{
	return Degree * (b2_pi / 180.0f);
}

static float RadianToDegree(float Radian)
{
	return Radian * (180.0f / b2_pi);
}

void PhysicsBody::InitPhysicsBody(std::vector<Node*>& Nodes, b2WorldId& PhysicsWorldID, b2BodyType BodyType, float Friction, float Density)
{
	b2BodyDef body_def = b2DefaultBodyDef();
	body_def.type = BodyType;
	body_def.rotation = b2MakeRot(DegreeToRadian(Angle));
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
	std::cout << m_Friction << '\n';
	shape_definition.friction = m_Friction;

	switch (_Collider->ColliderType)
	{
	case Collider::Type::BOX: {
		b2Vec2 box_offset = { _Collider->LocalPosition.x / PIXEL_TO_METER, _Collider->LocalPosition.y / PIXEL_TO_METER };
		b2Polygon box_shape = b2MakeOffsetBox(_Collider->BoxColliderSize.x, _Collider->BoxColliderSize.y, box_offset, b2MakeRot(DegreeToRadian(_Collider->Angle)));
		
		b2CreatePolygonShape(BodyID, &shape_definition, &box_shape);
	}	break;
	case Collider::Type::CIRCLE: {
		b2Circle circle_shape;
		circle_shape.radius = _Collider->CircleRadius;
		circle_shape.center = { _Collider->LocalPosition.x / PIXEL_TO_METER, _Collider->LocalPosition.y / PIXEL_TO_METER };
		b2CreateCircleShape(BodyID, &shape_definition, &circle_shape);
	}	break;
	case Collider::Type::POLYGON: {
		b2Transform polygon_transform;
		polygon_transform.p = { _Collider->LocalPosition.x / PIXEL_TO_METER, _Collider->LocalPosition.y / PIXEL_TO_METER };
		polygon_transform.q = b2MakeRot(DegreeToRadian(_Collider->Angle));
		b2Polygon polygon_shape = b2MakeOffsetPolygon(&_Collider->PolygonHull, 0, polygon_transform);

		b2CreatePolygonShape(BodyID, &shape_definition, &polygon_shape);
	}	break;
	default:
		break;
	}
}

void PhysicsBody::UpdatePhysicsNode()
{
	Position = b2Body_GetPosition(BodyID) * PIXEL_TO_METER;
	
	Angle = RadianToDegree(b2Rot_GetAngle(b2Body_GetRotation(BodyID)));
}

void PhysicsBody::DeletePhysicsBody()
{
	b2DestroyBody(BodyID);
}