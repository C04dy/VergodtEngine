#ifndef NODE_H
#define NODE_H

#pragma once

#include <iostream>
#include <vector>
#include "Vector2.h"

#include "VisualScripting/ScriptingNodes.h"

enum class NodeType {
	NODE,
	SPRITE,
	PHYSICSBODY,
	CAM
};

class InputManager;

class Node
{
public:
	Vector2 Position = Vector2(0, 0);
	Vector2 LocalPosition = Vector2(0, 0);
	Vector2 Size = Vector2(1, 1);
	float Angle = 0;

	void AddChild(Node* Child);

	void UpdateChild();

	std::string Name;

	NodeType Type = NodeType::NODE;

	VisualScript* Script = nullptr;
private:
	std::vector<Node*> m_childNodes;
};

#endif