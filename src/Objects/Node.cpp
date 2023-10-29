#include "Node.h"

void Node::AddChild(Node* c)
{
	m_childNode.push_back(c);
}

void Node::UpdateChild()
{
	for (int i = 0; i < (int)m_childNode.size(); i++)
	{
		m_childNode[i]->Transform.Position = Transform.Position;
		m_childNode[i]->Transform.Angle = Transform.Angle;
	}
}