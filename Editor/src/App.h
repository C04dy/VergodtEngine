#ifndef APP_H
#define APP_H

#pragma once

#include <stdexcept>
#include <iostream>
#include <string>
#include "imgui.h"
#include "Viewport.h"
#include "Scripting.h"
#include "Inspector.h"
#include "SceneView.h"
#include "Project.h"

class SDL_Window;
class SDL_Renderer;

class App
{
public:
    ~App();

    void Init();

    int Run();

    bool IsAppRunning() { return m_Running; }
private:
    void DockSpace();
private:
    bool m_Running = false;

    SDL_Window* m_Window = nullptr;

    SDL_Renderer* m_Renderer = nullptr;

    ImGuiIO* m_io = nullptr;

    Scripting m_Scripting;

    Viewport m_Viewport;

    Inspector m_Inspector;

    SceneView m_SceneView;

    Project m_Project;
};

#endif
