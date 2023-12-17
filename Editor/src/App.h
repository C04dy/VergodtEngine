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

    int m_scenefileindex = 0;

    std::vector<Node> Nodes;

    char* m_name = nullptr;
    float m_x = 0, m_y = 0; 

    void Scene(std::vector<Node> n);
};

#endif