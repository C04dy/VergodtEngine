#pragma once

#include <iostream>
#include <vector>
#include "Vector2.h"

class Transform{
public:
	Vector2 Position = Vector2(0, 0);
	Vector2 Size = Vector2(1, 1);
	float Angle = 0;
};

class Node
{
public:
	Transform Transform;

	void AddChild(Node* c);
	void UpdateChild();

	std::string Name;

	std::string Script;
protected:
	std::vector<Node*> m_childNode;
};

