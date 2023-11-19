#pragma once

#include <iostream>
#include <vector>
#include <box2d/b2_math.h>

using Vector2 = b2Vec2;

class Node
{
public:
	struct{
		Vector2 Position = Vector2(0, 0);
		Vector2 Size = Vector2(1, 1);
		float Angle = 0;
	}Transform;

	void AddChild(Node* c);
	void UpdateChild();

	std::string Name;

	std::string Script;

protected:
	std::vector<Node*> m_childNode;
};

