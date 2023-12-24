#pragma once

#include <iostream>
#include <vector>
#include "Vector2.h"

#include "../Engine/VisualScripting/ScriptingNodes.h"

class TransformClass{
public:
	Vector2 Position = Vector2(0, 0);
	Vector2 Size = Vector2(1, 1);
	float Angle = 0;
};

class Node
{
public:
	~Node();

	TransformClass Transform;

	void AddChild(Node* Child);
	
	void UpdateChild();

	std::string Name;

	VisualScript* Script = nullptr;
protected:
	std::vector<Node*> m_childNode;
};