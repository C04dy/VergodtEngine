#include "Node.h"
#include "PhysicsBody.h"

void Node::AddChild(Node* Child) {
	m_childNodes.push_back(Child);
	Child->LocalPosition = Position - Child->Position;

	Child->Position = Position + Child->LocalPosition;
	Child->Size = Size;
	Child->Angle = Angle;
	if (Child->Type == NodeType::PHYSICSBODY) {
		((PhysicsBody*)Child)->GetBody()->SetTransform(Child->Position / 100, Child->Angle);
	}
}

void Node::UpdateChild() {
	for (Node* n : m_childNodes) {
		n->Position = Position + n->LocalPosition;
		n->Size = Size;
		n->Angle = Angle;
	}
}