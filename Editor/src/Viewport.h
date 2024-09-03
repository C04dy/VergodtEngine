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

    void ViewportSpace(SDL_Renderer* renderer, const std::vector<Node>& nodes);

private:
    ImVec2 m_scrolling;
};

#endif