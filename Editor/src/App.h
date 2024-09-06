#ifndef APP_H
#define APP_H

#pragma once

#include <stdexcept>
#include <iostream>
#include <string>
#include "Viewport.h"
#include "imgui.h"
#include "Scripting.h"
#include "Inspector.h"
#include "SceneView.h"

class SDL_Window;
class SDL_Renderer;

struct Node {
public:
    struct NodeValue {
        enum class Type {
            NULLTYPE = 0,
            STRING = 1
        };
        void* Value = nullptr;
        Type ValueType = Type::NULLTYPE;

        NodeValue(void* value, Type type) {
            Value = value;
            ValueType = type;
        }
    };


    enum class Type {
        NODE = 0,
        SPRITE = 1,
        CAM = 2,
        PHYSICSBODY = 3
    };

    Type NodeType = Type::NODE;

    std::string Name = "Node", Script = "Null";

    ImVec2 Position, Size;

    float Angle;

    std::vector<NodeValue> NodeValues;

    bool IsChild = false;

    int ChildCount = 0;
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

    Inspector m_inspector;

    SceneView m_sceneview;

    std::vector<Node> m_nodes;
};

#endif
