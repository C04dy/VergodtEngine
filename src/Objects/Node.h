#ifndef NODE_H
#define NODE_H

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
	std::string Script;

	std::string ScriptClassName = "null";
	std::string ScriptLocation = "null";
	int ScriptIndex = -1;

	NodeType Type = NodeType::NODE;

	class Children {
	public:
		void AddChildToList(Node* n) { m_childNode.push_back(n); }
		Node* GetChild(int id) { return m_childNode[id]; }
		std::vector<Node*> GetChilds() { return m_childNode; }
	private:
		std::vector<Node*> m_childNode;
	};

	Children ChildNodes;

	//VisualScript* Script = nullptr;
};

#endif