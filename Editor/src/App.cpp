#include "App.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif
#include <fstream>
#include "StringFunctions.h"
#include <functional>
#include <nfd.h>
#include <SDL3_image/SDL_image.h>
#include "Viewport.h"
#include "Scripting.h"
#include "Inspector.h"
#include "SceneView.h"
#include "Project.h"
#include "process.hpp"

App::~App()
{
    NFD_Quit();

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    m_io = nullptr;

    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

void App::Init()
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) == SDL_FALSE)
    {
        std::cout << "SDL_Init failed : ";
        throw std::runtime_error(SDL_GetError());
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_AVIF | IMG_INIT_JXL | IMG_INIT_TIF | IMG_INIT_WEBP) < 0)
    {
        std::cout << "IMG_Init failed";
        throw std::runtime_error(SDL_GetError());
    }

    // Create window with SDL_Renderer graphics context
    Uint32 window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    m_Window = SDL_CreateWindow("VergodtEngine Editor", 1280, 720, window_flags);
    if (m_Window == nullptr)
    {
        std::cout << "SDL_CreateWindow() failed : ";
        throw std::runtime_error(SDL_GetError());
    }
    m_Renderer = SDL_CreateRenderer(m_Window, nullptr);
    SDL_SetRenderVSync(m_Renderer, 1);
    if (m_Renderer == nullptr)
    {
        std::cout << "SDL_CreateRenderer() failed : ";
        throw std::runtime_error(SDL_GetError());
    }
    SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(m_Window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO(); (void)m_io;
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 3));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 21);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 14);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 13);

    ImGui_ImplSDL3_InitForSDLRenderer(m_Window, m_Renderer);
    ImGui_ImplSDLRenderer3_Init(m_Renderer);

    m_io->Fonts->AddFontFromFileTTF("../Assets/JetBrainsMono-Medium.ttf", 15.0f);
    m_io->IniFilename = "../Assets/VergodtEngine.ini";

    NFD_Init();
    
    m_Project.SetRenderer(m_Renderer);

    //m_Project.LoadSceneFile("../Assets/FlappyBird/FlappyBird.vscene");
    //m_Project.SaveSceneFile();

    // Main loop
    m_Running = true;
}

int App::Run() {
    int selected_node = -1;
    std::string ConsoleLine;

    SDL_Event event;
#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (IsAppRunning())
#endif
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                m_Running = false;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(m_Window))
                m_Running = false;
            if (event.type == SDL_EVENT_DROP_FILE && m_Project.ProjectInitilized)
            {
                m_FileSystem.CopyDroppedFile(event.drop.data, m_Project);
            }
        }

        if (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Do stuff here
        static bool sdw = true; ImGui::ShowDemoWindow(&sdw);

        DockSpace();

        if (!m_Project.ProjectInitilized)
        {
            if (m_Project.InitilizeProject())
            {
                SDL_MaximizeWindow(m_Window);
                m_FileSystem.SetProjectDirectory(m_Project.GetProjectLocation(), m_Project);
            }
        }
        else
        {
            m_Scripting.ScriptingSpace();

            m_Viewport.ViewportSpace(m_Renderer, m_Project.Nodes, selected_node, m_Project.SavedProject, m_Project);

            m_SceneView.SceneViewSpace(m_Project.Nodes, selected_node, m_Project.SavedProject);

            m_Inspector.InspectorSpace(m_Project.Nodes, selected_node, m_Scripting, m_Project.SavedProject, m_Project);

            m_FileSystem.FileSystemSpace(m_Project);

            if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_S))
            {
                if (m_Project.SaveSceneFile(m_FileSystem))
                    m_Project.SavedProject = true;
            }

            ImGui::Begin("Debug");
            if (ImGui::Button("Play"))
            {
#ifdef _WIN32
                std::string engine = "VergodtEngine.exe ";
#else
                std::string engine = "./VergodtEngine ";
#endif
                // i will fix this surely someday..
                TinyProcessLib::Process process(engine + m_Project.GetProjectLocation() + m_Project.GetProjectFile(), "",
                    [](const char* bytes, size_t n)
                    {
                        std::cout << "Game output: " << std::string(bytes, n);
                    },
                    [](const char *bytes, size_t n) {
                    }
                );

                int exit_status = process.get_exit_status();
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColorFloat(m_Renderer, 0.45f, 0.55f, 0.60f, 1.00f);
        SDL_RenderClear(m_Renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
        SDL_RenderPresent(m_Renderer);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    return 0;
}

void App::DockSpace()
{
    static bool opt_padding = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            //ImGui::MenuItem("Padding", NULL, &opt_padding);
            //ImGui::Separator();

            //if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
            //if (ImGui::MenuItem("Flag: NoDockingSplit",         "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
            //if (ImGui::MenuItem("Flag: NoUndocking",            "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
            //if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                   { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            //if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            //if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, true)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}