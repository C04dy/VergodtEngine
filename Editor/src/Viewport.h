#ifndef VIEWPORT_H
#define VIEWPORT_H

#pragma once
#include "imgui.h"
#include <vector>

struct Node;
class SDL_Renderer;
class Project;

class Viewport
{
public:
    Viewport() = default;

    void ViewportSpace(SDL_Renderer* Renderer, std::vector<Node>& Nodes, int& SelectedNode, bool& Saved, Project& CurrentProject);
private:
    ImVec2 m_Scrolling;

    float m_Zoom = 1;
};

#endif