#ifndef NODE_H
#define NODE_H

#pragma once

#include <iostream>
#include <vector>
#include "Vector2.h"

#include "VisualScripting/ScriptingNodes.h"

class InputManager;

class Node
{
public:
	enum class Type
	{
		NODE = 0,
		SPRITE = 1,
		PHYSICSBODY = 2,
		CAM = 3
	};
public:
	Vector2 Position = Vector2(0, 0);
	Vector2 LocalPosition = Vector2(0, 0);
	Vector2 Size = Vector2(1, 1);
	float Angle = 0;

	void AddChild(Node* Child);

	void UpdateChild();

	std::string Name;

	Type Type = Type::NODE;

	VisualScript* Script = nullptr;
private:
	std::vector<Node*> m_ChildNodes;
};

#endif