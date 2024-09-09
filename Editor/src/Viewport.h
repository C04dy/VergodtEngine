#ifndef VIEWPORT_H
#define VIEWPORT_H

#pragma once
#include "imgui.h"
#include <vector>

struct Node;
class SDL_Renderer;

class Viewport
{
public:
    Viewport();
    ~Viewport();

    void ViewportSpace(SDL_Renderer* Renderer, std::vector<Node>& Nodes, int& SelectedNode);
private:
    ImVec2 m_Scrolling;
};

#endif