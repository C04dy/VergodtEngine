#ifndef NODE_H
#define NODE_H

#pragma once

#include <iostream>
#include <vector>
#include <simplesquirrel/simplesquirrel.hpp>
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
		
		ssq::Object GetChild(int id) { 
			return vm->callFunc(vm->findFunc("GetNodeFromList"), *vm, m_childNode[id]->ScriptIndex);
		}
		
		std::vector<Node*> GetChilds() { return m_childNode; }
		
		int GetChildCount() { return m_childNode.size(); }
		
		ssq::VM* vm;
	private:
		std::vector<Node*> m_childNode;
	};

	Children ChildNodes;

	//VisualScript* Script = nullptr;
};

#endif