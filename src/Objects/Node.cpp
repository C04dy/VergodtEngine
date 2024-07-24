#include "Node.h"
#include "PhysicsBody.h"

void Node::AddChild(Node* Child) {
	m_childNode.push_back(Child);
	Child->LocalPosition = Child->Position;

	for (Node* n : m_childNode) {	
		n->Position = Position + n->LocalPosition;
		n->Size = Size;
		n->Angle = Angle;
		if (n->Type == NodeType::PHYSICSBODY) {
			((PhysicsBody*)n)->GetBody()->SetTransform(n->Position / 100, n->Angle);
		}
	}
}

void Node::UpdateChild() {
	for (Node* n : m_childNode) {
		n->Position = Position + n->LocalPosition;
		n->Size = Size;
		n->Angle = Angle;
	}
}