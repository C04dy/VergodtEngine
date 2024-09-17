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

int Node::s_IDs = 0;

App::~App()
{
    for (Node node : m_Nodes)
        node.NodeValues.clear();

    NFD_Quit();

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    m_io = nullptr;

    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

static std::string GetNodeTypeAsString(Node::Type NodeType)
{
    switch (NodeType)
    {
    case Node::Type::NODE:
        return "NODE";
        break;
    case Node::Type::SPRITE:
        return "SPRITE";
        break;
    case Node::Type::CAM:
        return "CAM";
        break;
    case Node::Type::PHYSICSBODY:
        return "PHYSICSBODY";
        break;
    case Node::Type::COLLIDER:
        return "COLLIDER";
        break;
    }
}

void SaveNodes(std::string& Line, const std::vector<Node>& Nodes, bool IsChild = false)
{
    for (int i = 0; i < static_cast<int>(Nodes.size()); i++)
    {
        if (Nodes[i].IsChild && !IsChild)
            continue;

        Line += "[NODETYPE=";
        Line += GetNodeTypeAsString(Nodes[i].NodeType);
        Line += "] ";

        Line += "[NAME=";
        Line += Nodes[i].Name;
        Line += "] ";

        if (Nodes[i].Position.x != 0.0f || Nodes[i].Position.y != 0.0f)
        {
            Line += "[POSITION(";
            Line += Nodes[i].Position.x;
            Line += ',';
            Line += Nodes[i].Position.y;
            Line += ")] ";
        }
        if (Nodes[i].Size.x != 0.0f || Nodes[i].Size.y != 0.0f)
        {
            Line += "[SIZE(";
            Line += Nodes[i].Size.x;
            Line += ',';
            Line += Nodes[i].Size.y;
            Line += ")] ";
        }
        if (Nodes[i].Angle != 0.0f)
        {
            Line += "[ANGLE(";
            Line += Nodes[i].Angle;
            Line += ")] ";
        }

        switch (Nodes[i].NodeType)
        {
        case Node::Type::SPRITE:
            Line += "[ASSET=";
            Line += *(std::string*)Nodes[i].NodeValues[0]->Value;
            Line += "] ";
            break;
        case Node::Type::PHYSICSBODY:
            //Line += "[PHYSICSTYPE=";
            //Line += *(std::string*)Nodes[i].NodeValues[0]->Value;
            //Line += "] ";
            break;
        default:
            break;
        }
    }
}

void App::SaveSceneFile()
{
    std::string line;

    SaveNodes(line, m_Nodes);

    std::ofstream write_file(m_CurrentScene);
    write_file << line;
}

void App::LoadSceneFile(const std::string& FilePath)
{
    std::string line;
    std::ifstream scene_file(FilePath);

    if (scene_file.fail())
    {
        std::cout << "Scene File did not found.\n";
        return;
    }

    m_CurrentScene = FilePath;

    int line_count = 0;
    while (std::getline(scene_file, line))
    {
        if (line[0] != '#' || line[0] != '$')
        {
            line_count += 1;
            m_Nodes.push_back(Node());

            std::string type = GetLineBetween(line, "[NODETYPE=", "]");

            if (type == "NODE")
            {
                m_Nodes[m_Nodes.size() - 1].NodeType = Node::Type::NODE;
            }
            else if (type == "PHYSICSBODY")
            {
                m_Nodes[m_Nodes.size() - 1].NodeType = Node::Type::PHYSICSBODY;
            }
            else if (type == "SPRITE")
            {
                m_Nodes[m_Nodes.size() - 1].NodeType = Node::Type::SPRITE;
                if (IsLineExist(line, "[ASSET="))
                {
                    m_Nodes[m_Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new std::string(GetLineBetween(line, "[ASSET=", "]")), Node::NodeValue::Type::STRING));
                }
                else
                {
                    m_Nodes[m_Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new std::string("None"), Node::NodeValue::Type::STRING));
                }
            }
            else if (type == "CAMERA")
            {
                m_Nodes[m_Nodes.size() - 1].NodeType = Node::Type::CAM;
            }
            else if (type == "COLLIDER")
            {
                m_Nodes[m_Nodes.size() - 1].NodeType = Node::Type::COLLIDER;


                if (IsLineExist(line, "[COLLIDERTYPE="))
                {
                    std::string collider_type = GetLineBetween(line, "[COLLIDERTYPE=", "]");

                    if (collider_type == "BOX")
                    {
                        m_Nodes[m_Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::ColliderType(Node::ColliderType::BOX), Node::NodeValue::Type::INT, { "Box", "Circle", "Polygon" }));

                        std::string size = GetLineBetween(line, "[COLLIDERSIZE(", ")]");
                        m_Nodes[m_Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new ImVec2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ","))), Node::NodeValue::Type::VECTOR2));
                    }
                    else if (collider_type == "CIRCLE")
                    {
                        m_Nodes[m_Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::ColliderType(Node::ColliderType::CIRCLE), Node::NodeValue::Type::INT, { "Box", "Circle", "Polygon" }));

                        m_Nodes[m_Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new float(std::stof(GetLineBetween(line, "[RADIUS=(", ")]"))), Node::NodeValue::Type::FLOAT));
                    }
                    else if (collider_type == "POLYGON")
                    {
                        m_Nodes[m_Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::ColliderType(Node::ColliderType::POLYGONS), Node::NodeValue::Type::INT, { "Box", "Circle", "Polygon" }));

                        std::vector<void*> points;

                        int point_count = std::stoi(GetLineBetween(line, "[POINTCOUNT=(", ")]"));

                        std::string polygon_points = GetLineBetween(line, "[POINTS=", "]");

                        for (int i = 0; i < point_count; i++)
                        {
                            points.push_back(new ImVec2(std::stof(GetLineBetween(polygon_points, "(", ",")), std::stof(GetLineBetween(polygon_points, ",", ")"))));

                            if (i != point_count)
                                polygon_points.erase(0, GetLineBetween(polygon_points, "(", ")").size() + 3);
                            else
                                polygon_points.erase(0, GetLineBetween(polygon_points, "(", ")").size() + 2);
                        }

                        m_Nodes[m_Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(points));
                    }
                }
            }

            if (IsLineExist(line, "[POSITION("))
            {
                std::string pose = GetLineBetween(line, "[POSITION(", ")]");
                m_Nodes[m_Nodes.size() - 1].Position = ImVec2(std::stof(GetLineBetween(pose, 0, ",")), std::stof(GetLineBetween(pose, ",")));
            }
            if (IsLineExist(line, "[SIZE("))
            {
                std::string size = GetLineBetween(line, "[SIZE(", ")]");
                m_Nodes[m_Nodes.size() - 1].Size = ImVec2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ",")));
            }

            if (IsLineExist(line, "[ANGLE("))
            {
                m_Nodes[m_Nodes.size() - 1].Angle = std::stof(GetLineBetween(line, "[ANGLE(", ")]"));
            }

            if (IsLineExist(line, "[SCRIPT="))
            {
                m_Nodes[m_Nodes.size() - 1].Script = GetLineBetween(line, "[SCRIPT=", "]");
            }

            if (IsLineExist(line, "[CHILDINDEX="))
            {
                std::string ci = GetLineBetween(line, "[CHILDINDEX=(", ")]");
                if (IsLineExist(ci, ",")) {
                    for (int i = 0; i < static_cast<int>(ci.size()); i++)
                    {
                        if (ci[i] != ',')
                        {
                            m_Nodes[m_Nodes.size() - 1].ChildIDs.push_back(ci[i] - '0');
                            m_Nodes[ci[i] - '0'].IsChild = true;
                            m_Nodes[ci[i] - '0'].ParentID = m_Nodes[m_Nodes.size() - 1].ID;
                        }
                    }
                }
                else
                {
                    m_Nodes[m_Nodes.size() - 1].ChildIDs.push_back(std::stoi(ci));
                    m_Nodes[std::stoi(ci)].IsChild = true;
                    m_Nodes[std::stoi(ci)].ParentID = m_Nodes[m_Nodes.size() - 1].ID;
                }
            }
            m_Nodes[m_Nodes.size() - 1].Name = GetLineBetween(line, "[NAME=", "]");
        }
    }
}

void App::Init()
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0)
    {
        std::cout << "SDL_Init failed : ";
        throw std::runtime_error(SDL_GetError());
    }

    // Create window with SDL_Renderer graphics context
    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
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
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::LoadIniSettingsFromDisk("../Assets/VergodtEngine.ini");
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(m_Window, m_Renderer);
    ImGui_ImplSDLRenderer3_Init(m_Renderer);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../Assets/JetBrainsMono-Medium.ttf", 16.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    m_io->Fonts->AddFontFromFileTTF("../Assets/JetBrainsMono-Medium.ttf", 15.0f);
    m_io->IniFilename = "../Assets/VergodtEngine.ini";

    NFD_Init();
    
    // Load Scene

    LoadSceneFile("../Assets/FlappyBird/FlappyBird.vscene");

    // Main loop
    m_Running = true;
}

int App::Run() {
    int selected_node = -1;

#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (IsAppRunning())
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                m_Running = false;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(m_Window))
                m_Running = false;
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
        bool sdw = true; ImGui::ShowDemoWindow(&sdw);

        DockSpace();

        m_Scripting.ScriptingSpace();

        m_Viewport.ViewportSpace(m_Renderer, m_Nodes, selected_node);

        m_SceneView.SceneViewSpace(m_Nodes, selected_node);

        m_Inspector.InspectorSpace(m_Nodes, selected_node, &m_Scripting);

        // Rendering
        ImGui::Render();
        //SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
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