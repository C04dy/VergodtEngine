#include "Node.h"
#include "PhysicsBody.h"

void Node::AddChild(Node* Child) {
	ChildNodes.AddChildToList(Child);
	Child->LocalPosition = Child->Position;

	for (Node* n : ChildNodes.GetChilds()) {	
		n->Position = Position + n->LocalPosition;
		n->Size = Size;
		n->Angle = Angle;
		if (n->Type == NodeType::PHYSICSBODY) {
			((PhysicsBody*)n)->GetBody()->SetTransform(n->Position / 100, n->Angle);
		}
	}
}

void Node::UpdateChild() {
	for (Node* n : ChildNodes.GetChilds()) {
		if (n->Type == NodeType::PHYSICSBODY) {
			n->LocalPosition = Position - n->Position;
			((PhysicsBody*)n)->GetBody()->SetTransform(Position + n->LocalPosition / 100, n->Angle);
		}
		n->Position = Position + n->LocalPosition;
		n->Size = Size;
		n->Angle = Angle;
	}
}