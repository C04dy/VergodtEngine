#include "Node.h"

Node::~Node(){
	/*delete Script;
	Script = nullptr;*/
}

void Node::AddChild(Node* Child){
	m_childNode.push_back(Child);
}

void Node::UpdateChild(){
	for(int i = 0; i < (int)m_childNode.size(); i++){	
		m_childNode[i]->Transform.Position = Transform.Position;
		m_childNode[i]->Transform.Size = Transform.Size;
		m_childNode[i]->Transform.Angle = Transform.Angle;
	}
}