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
    Node(float X, float Y, std::string Name){
        posX = X;
        posY = Y;
        this->Name = Name;
    }

    float posX = 0, posY = 0;

    std::string Name = "";

    std::vector<Node> ChildNodes;
    
    bool IsChild = false;
    bool HaveChild = false;
};

class App
{
public:
    App();
    ~App();

    void InitApp();
    void RunApp();
private:
    std::vector<std::string> m_lines;
    std::ifstream m_scenefile;

    int m_nodeindex = 0;

    std::vector<Node> Nodes;

    void Scene(std::vector<Node> n);
    void AddChildNodes(std::vector<Node> n, int offset);
};

#endif