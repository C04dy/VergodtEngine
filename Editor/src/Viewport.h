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

    void ViewportSpace(SDL_Renderer* renderer, std::vector<Node>& nodes, int& selectednode);

private:
    ImVec2 m_scrolling;
};

#endif