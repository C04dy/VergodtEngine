#pragma once

#include <iostream>
#include <vector>
#include "Vector2.h"

//#include "../Engine/VisualScripting/ScriptingNodes.h"

class Node
{
public:
	~Node();

	Vector2 Position = Vector2(0, 0);
	Vector2 Size = Vector2(1, 1);
	float Angle = 0;

	void AddChild(Node* Child);

	std::string GetName() const { return Name; }
	Vector2 GetPosition() const { return Position; }

	void SetName(std::string NewName) { Name = NewName; }

	void UpdateChild();

	std::string Name;

	std::string ScriptClassName = "null";
	std::string ScriptLocation = "null";
	int ScriptIndex = -1;

	//VisualScript* Script = nullptr;
protected:
	std::vector<Node*> m_childNode;
};