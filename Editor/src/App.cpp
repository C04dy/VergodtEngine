#include "App.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "SceneFileFunctions.h"

App::App()
{

}

App::~App()
{

}

void App::InitApp(){
    m_scenefile.open("../../Assets/testFORGUI.vscene");
    std::string ln;
    while (std::getline(m_scenefile, ln)){
        m_lines.push_back(ln);

        std::string pos = GetLineBetween(ln, "[POSITION(", ")]");

        Nodes.push_back(Node(std::stof(pos.substr(0, pos.find(",")).c_str()),
                            std::stof(pos.substr(pos.find(",") + 1, pos.length() - pos.find(",")).c_str()),
                            GetLineBetween(ln, "[NAME=", "]")));
        
        if(GetLineBetween(ln, "[CHILD=", "]") != "0"){
            for (int i = 0; i < std::stoi(GetLineBetween(ln, "[CHILD=", "]")); i++)
            {   
                Nodes[Nodes.size() - (i + 2)].IsChild = true;
                Nodes[Nodes.size() - 1].ChildNodes.push_back(Nodes[Nodes.size() - (i + 2)]);
            }
            Nodes[Nodes.size() - 1].HaveChild = true;
        }
    }
}

void App::RunApp()
{
    // Docking
    {
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        bool p_open = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &p_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
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
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
                if (ImGui::MenuItem("Flag: NoDockingSplit",         "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
                if (ImGui::MenuItem("Flag: NoUndocking",            "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
                if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                   { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();

                if (ImGui::MenuItem("Close", NULL))
                    p_open = false;
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    ImGui::ShowDemoWindow(new bool(true));
    // Inspector
    {
        ImGui::Begin("Inspector");

        char* n = (char*)Nodes[m_nodeindex].Name.c_str();
        ImGui::InputText("Name", n, 1000);
        Nodes[m_nodeindex].Name = n;

        ImGui::Text("Transform");
        ImGui::Text("Position");
        ImGui::InputFloat("X", &Nodes[m_nodeindex].posX);
        ImGui::InputFloat("Y", &Nodes[m_nodeindex].posY);

        ImGui::End();
    }
    // Scene Hierarchy
    {
        ImGui::Begin("Hierarchy");
        
        Scene(Nodes);

        ImGui::End();
    }
}

void App::AddChildNodes(std::vector<Node> n, int offset){
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int selection_mask = (1 << 2);
    for (int i = 0; i < (int)n.size(); i++)
    {
        ImGuiTreeNodeFlags node_flags = base_flags;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        if(n[i].HaveChild){
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, n[i].Name.c_str());
            if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                m_nodeindex = (offset - 1) - i;
            if(ImGui::BeginDragDropSource()){
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
            if(node_open){
                
                AddChildNodes(n[i].ChildNodes, (offset - 1) - i);

                ImGui::TreePop();
            }
        }
        else if(!n[i].HaveChild){
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, n[i].Name.c_str());
            if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                m_nodeindex = (offset - 1) - i;
            if(ImGui::BeginDragDropSource()){
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
        }
    }
}

void App::Scene(std::vector<Node> n){
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int selection_mask = (1 << 2);
    for (int i = 0; i < (int)n.size(); i++)
    {
        ImGuiTreeNodeFlags node_flags = base_flags;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        if(!n[i].IsChild && n[i].HaveChild){
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, n[i].Name.c_str());
            if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                m_nodeindex = i;
            if(ImGui::BeginDragDropSource()){
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
            if(node_open){
                
                AddChildNodes(n[i].ChildNodes, i);

                ImGui::TreePop();
            }
        }
        else if(!n[i].IsChild && !n[i].HaveChild){
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, n[i].Name.c_str());
            if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                m_nodeindex = i;
            if(ImGui::BeginDragDropSource()){
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
        }
    }
}