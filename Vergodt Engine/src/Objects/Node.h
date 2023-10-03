#pragma once

#include "Math/Vector2.h"
#include <iostream>
#include <vector>

class Node
{
public:

	Vector2 LocalPosition = Vector2(0, 0);

	struct
	{
		Vector2 Position = Vector2(0, 0);
		Vector2 Size = Vector2(1, 1);
		float Angle;
	}Transform;

	void AddChild(Node* c);
	void UpdateChild();

protected:
	std::vector<Node*> m_childNode;
};

