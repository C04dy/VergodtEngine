#pragma once

#include <iostream>
#include <vector>
#include "Vector2.h"

//#include "../Engine/VisualScripting/ScriptingNodes.h"

enum class NodeType {
	NODE,
	SPRITE,
	PHYSICSBODY,
	CAM
};

class Node
{
public:
	Node(const std::string& name);

	Vector2 Position = Vector2(0, 0);
	Vector2 Size = Vector2(1, 1);
	float Angle = 0;

	void AddChild(Node* Child);

	void UpdateChild();

	std::string Name;

	std::string ScriptClassName = "null";
	std::string ScriptLocation = "null";
	int ScriptIndex = -1;

	NodeType Type = NodeType::NODE;

	//VisualScript* Script = nullptr;
protected:
	std::vector<Node*> m_childNode;
};