#ifndef APP_H
#define APP_H

#pragma once
#define SDL_MAIN_HANDLED
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Node{
public:
    //BASE
    Node(float posX, float posY, float SizeX, float SizeY, float Angle, std::string Name){
        this->posX = posX;
        this->posY = posY;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->Name = Name;
    }

    enum class NodeType{
        NODE,
        SPRITE,
        CAMERA,
        PHYSICSBODY
    };

    float posX = 0, posY = 0;
    float sizeX = 0, sizeY = 0;
    float Angle = 0;

    std::string Name = "";

    int ChildNodes = 0;
    
    bool IsChild = false;
    bool HaveChild = false;

    //SPRITE
    std::string SpritePath = "";

    NodeType NodesType = NodeType::NODE;
};

class App
{
public:
    void InitApp();
    void RunApp();
    void EndApp();
private:
    std::vector<std::string> m_lines;

    int m_nodeindex = 0;

    std::vector<Node> Nodes;

    void Scene();
    void AddChildNodes(int ChildSize, int offset);
};

#endif