#pragma once

#include <iostream>
#include <vector>
#include "Vector2.h"

class TransformClass{
public:
	Vector2 Position = Vector2(0, 0);
	Vector2 Size = Vector2(1, 1);
	float Angle = 0;
};

class Node
{
public:
	TransformClass Transform;

	void AddChild(Node* Child);
	
	void UpdateChild();

	std::string Name;

	std::string Script;
protected:
	std::vector<Node*> m_childNode;
};