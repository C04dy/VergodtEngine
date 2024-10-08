#include "Node.h"
#include "PhysicsBody.h"

int Node::s_IDs = 0;

Node::~Node()
{
	if (Script != nullptr)
	{
		delete Script;
		Script = nullptr;
	}
}

void Node::AddChild(Node* Child)
{
	m_ChildIDs.push_back(Child->ID);
	Child->LocalPosition = Child->Position - Position;
	Child->LocalSize = Child->Size / Size;

	Child->Position = Position + Child->LocalPosition;
	Child->Size = Size;
	Child->Angle = Angle;

	Child->ParentID = ID;

	switch (Child->NodeType)
	{
	case Type::PHYSICSBODY:
		b2Body_SetTransform(((PhysicsBody*)Child)->BodyID, Child->Position, b2MakeRot(Child->Angle));
		break;
	default:
		break;
	} 
}

void Node::UpdateChild(std::vector<Node*>& Nodes)
{
	for (Node* node : Nodes)
	{
		for (int child_id : m_ChildIDs)
		{
			if (node->ID == child_id)
			{
				node->Position = Position + node->LocalPosition;
				node->Size = Size * node->LocalSize;
				node->Angle = Angle;
			}
		}
	}
}

void Node::SetID()
{
	ID = s_IDs;
	s_IDs += 1;
}