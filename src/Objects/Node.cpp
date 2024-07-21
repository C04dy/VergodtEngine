#include "Node.h"

void Node::AddChild(Node* Child) {
	m_childNode.push_back(Child);
}

void Node::UpdateChild() {
	for (int i = 0; i < (int)m_childNode.size(); i++) {	
		m_childNode[i]->Position = Position;
		m_childNode[i]->Size = Size;
		m_childNode[i]->Angle = Angle;
	}
}