#include "App.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "SceneFileFunctions.h"

#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>

void App::InitApp(){
    std::ifstream SceneFile("../../Assets/testFORGUI.vscene");
    std::string ln;
    while (std::getline(SceneFile, ln)){
        m_lines.push_back(ln);
        std::string pose = GetLineBetween(ln, "[POSITION(", ")]");
        std::string size = GetLineBetween(ln, "[SIZE(", ")]");
        std::string angle = GetLineBetween(ln, "[ANGLE(", ")]");

        Nodes.push_back(Node(
            std::stof(pose.substr(0, pose.find(",")).c_str()),
            std::stof(pose.substr(pose.find(",") + 1, pose.length() - pose.find(",")).c_str()),
            std::stof(size.substr(0, size.find(",")).c_str()),
            std::stof(size.substr(size.find(",") + 1, size.length() - size.find(",")).c_str()),   
            std::stof(angle.c_str()),
            GetLineBetween(ln, "[NAME=", "]"))
            );
        
        if(GetLineBetween(ln, "[CHILD=", "]") != "0"){
            for (int i = 0; i < std::stoi(GetLineBetween(ln, "[CHILD=", "]")); i++)
            {   
                Nodes[Nodes.size() - (i + 2)].IsChild = true;
                Nodes[Nodes.size() - 1].ChildNodes++;
            }
            Nodes[Nodes.size() - 1].HaveChild = true;
        }

        if(GetLineBetween(ln, "[NODETYPE=", "]") == "SPRITE"){
            Nodes[Nodes.size() - 1].NodesType = Node::NodeType::SPRITE;
            Nodes[Nodes.size() - 1].SpritePath = GetLineBetween(ln, "[ASSET=", "]");
        }
    }

    SceneFile.close();

    NFD_Init();
}

void InspectorNodeBase(Node& n){
    char* na = (char*)n.Name.c_str();
    ImGui::InputText("Name", na, 1000);
    n.Name = na;

    if(ImGui::TreeNode((void*)(intptr_t)0, "Transform")){
        if(ImGui::TreeNode((void*)(intptr_t)0, "Position")){
            ImGui::InputFloat("X", &n.posX);
            ImGui::InputFloat("Y", &n.posY);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode((void*)(intptr_t)1, "Size")){
            ImGui::InputFloat("X", &n.sizeX);
            ImGui::InputFloat("Y", &n.sizeY);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode((void*)(intptr_t)2, "Angle")){
            ImGui::InputFloat(" ", &n.Angle);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}

void InspectorNodeSprite(Node& n){
    if(ImGui::TreeNode((void*)(intptr_t)1, "Sprite")){
        if(ImGui::Button(n.SpritePath.c_str())){
            nfdchar_t* outPath;
            nfdfilteritem_t filterItem[1] = { "Image Files", "png,jpg,jpeg,avif,jxl,tif,webp" };
            nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 2, NULL);
            if(result == NFD_OKAY){
                puts(outPath);
                n.SpritePath = outPath;
            }
            else if(result == NFD_ERROR){
                printf("Error: %s\n", NFD_GetError());
            }
            NFD_FreePath(outPath);
        }

        ImGui::TreePop();
    }
}

void App::RunApp()
{
    //ImGui::ShowDemoWindow(new bool(true));
    // Inspector
    {
        ImGui::Begin("Inspector");

        InspectorNodeBase(Nodes[m_nodeindex]);

        switch (Nodes[m_nodeindex].NodesType)
        {
        case Node::NodeType::SPRITE:
            InspectorNodeSprite(Nodes[m_nodeindex]);
            break;
        }

        ImGui::End();
    }
    // Scene Hierarchy
    {
        ImGui::Begin("Hierarchy");
        
        Scene();

        ImGui::End();
    }
    // Filesystem
    {
        ImGui::Begin("Filesystem");

        ImGui::End();
    }
    // Viewport
    {
        ImGui::Begin("Viewport");

        ImGui::End();
    }
}

void App::EndApp(){
    NFD_Quit();
}

void App::AddChildNodes(int ChildSize, int offset){
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int selection_mask = (1 << 2);
    for (int i = 0; i < ChildSize; i++)
    {
        ImGuiTreeNodeFlags node_flags = base_flags;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if(is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        if(Nodes[(offset - 1) - i].HaveChild){
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, Nodes[(offset - 1) - i].Name.c_str());
            if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                m_nodeindex = (offset - 1) - i;
            if(ImGui::BeginDragDropSource()){
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
            if(node_open){
                
                AddChildNodes(Nodes[(offset - 1) - i].ChildNodes, (offset - 1) - i);

                ImGui::TreePop();
            }
        }
        else if(!Nodes[(offset - 1) - i].HaveChild){
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, Nodes[(offset - 1) - i].Name.c_str());
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

void App::Scene(){
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int selection_mask = (1 << 2);
    for (int i = 0; i < (int)Nodes.size(); i++)
    {
        ImGuiTreeNodeFlags node_flags = base_flags;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        if(!Nodes[i].IsChild && Nodes[i].HaveChild){
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, Nodes[i].Name.c_str());
            if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                m_nodeindex = i;
            if(ImGui::BeginDragDropSource()){
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
            if(node_open){
                
                AddChildNodes(Nodes[i].ChildNodes, i);

                ImGui::TreePop();
            }
        }
        else if(!Nodes[i].IsChild && !Nodes[i].HaveChild){
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, Nodes[i].Name.c_str());
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