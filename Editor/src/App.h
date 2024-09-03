#ifndef APP_H
#define APP_H

#pragma once

#include <stdexcept>
#include <iostream>
#include "Viewport.h"
#include "imgui.h"
#include "Scripting.h"

class SDL_Window;
class SDL_Renderer;

struct Node {
    enum class NodeType {
        NODE = 0,
        SPRITE = 1,
        CAM = 2,
        PHYSICSBODY = 3
    };

    NodeType Type = NodeType::NODE;

    std::string Name, Script;

    ImVec2 Position, Size;

    float Angle;
};

class App
{
public:
    ~App();
    
    void Init();

    int Run();

    bool IsAppRunning() { return m_running; }

    void LoadSceneFile(const std::string& FilePath);
    
private:
    bool m_running = false;

    SDL_Window* m_window = nullptr;

    SDL_Renderer* m_renderer = nullptr;

    ImGuiIO* io = nullptr;

    Scripting m_scripting;

    Viewport m_viewport;

    std::vector<Node> m_nodes;
};

#endif