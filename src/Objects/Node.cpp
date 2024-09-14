#include "Node.h"
#include "PhysicsBody.h"

int Node::s_IDs = 0;

void Node::AddChild(Node* Child)
{
	m_ChildIDs.push_back(Child->ID);
	Child->LocalPosition = Position - Child->Position;

	Child->Position = Position + Child->LocalPosition;
	Child->Size = Size;
	Child->Angle = Angle;

	Child->ParentID = ID;

	switch (Child->NodeType)
	{
	case Type::PHYSICSBODY:
		((PhysicsBody*)Child)->GetBody()->SetTransform(Child->Position / 100, Child->Angle);
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
				node->Size = Size;
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