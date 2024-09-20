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
		CAM = 3,
		COLLIDER = 4
	};
public:
	virtual ~Node();
	
	void AddChild(Node* Child);

	void UpdateChild(std::vector<Node*>& Nodes);

	void SetID();
public:
	int ID;

	Vector2 Position = Vector2(0, 0);
	Vector2 LocalPosition = Vector2(0, 0);
	Vector2 Size = Vector2(1, 1);
	Vector2 LocalSize = Vector2(1, 1);
	float Angle = 0;

	std::string Name = "Node";

	Type NodeType = Type::NODE;

	VisualScript* Script = nullptr;

	int ParentID = -1;
protected:
	std::vector<int> m_ChildIDs;

	static int s_IDs;
};

#endif