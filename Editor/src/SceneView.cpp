#include "SceneView.h"
#include "imgui.h"
#include "App.h"
//#include <algorithm>
#include <bits/stdc++.h>

void SceneView::SceneViewSpace(std::vector<Node>& Nodes, int& SelectedNode)
{
    ImGui::Begin("Scene");

    CreateTreeNodes(Nodes, SelectedNode);
    
    ImGui::End();
}

void SceneView::CreateTreeNodes(std::vector<Node>& Nodes, int& SelectedNode)
{
    static int selection_mask = (1 << 2);

    ImGui::BeginChild("NodeTree", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);

    for (int i = 0; i < static_cast<int>(Nodes.size()); i++)
    {
        if (Nodes[i].IsChild)
            continue;
        ImGui::PushID(i + 1);
        ImGui::BeginGroup();

        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;

        if (static_cast<int>(Nodes[i].ChildIDs.size()) == 0)
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool node_open = false;

        if (!Nodes[i].IsChild)
            node_open = ImGui::TreeNodeEx((void*)(int*)&i, node_flags, Nodes[i].Name.c_str());

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            SelectedNode = i;

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("SCENEVIEW_TREENODE", &SelectedNode, sizeof(SelectedNode), ImGuiCond_Once);
            ImGui::Text(Nodes[SelectedNode].Name.c_str());
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("SCENEVIEW_TREENODE"))
            {
                int drag_index = *(int*)p->Data;

                if (drag_index != i)
                {
                    int from = drag_index;
                    int to = i;

                    bool is_child = Nodes[from].IsChild;

                    if (Nodes[from].IsChild)
                    {
                        int parent_index = 0;
                        for (int j = 0; j < static_cast<int>(Nodes.size()); j++)
                        {   
                            if (Nodes[j].ID == Nodes[from].ParentID)
                            {
                                parent_index = j;
                                break;
                            }
                        }

                        auto it = std::find(Nodes[parent_index].ChildIDs.begin(), Nodes[parent_index].ChildIDs.end(), Nodes[from].ID);
                        if (it != Nodes[parent_index].ChildIDs.end())
                            Nodes[parent_index].ChildIDs.erase(it);

                        Nodes[from].ParentID = -1;
                        Nodes[from].IsChild = false;

                        if (!Nodes[to].IsChild)
                            ImGui::TreePop();
                    }

                    if (from < to)
                        std::rotate(Nodes.begin() + from, Nodes.begin() + from + 1, Nodes.begin() + to + 1);
                    else
                        std::rotate(Nodes.begin() + to, Nodes.begin() + from, Nodes.begin() + from + 1);

                    ImGui::EndGroup();
                    if (!is_child)
                        ImGui::PopID();
                    break;
                }
            }
            ImGui::EndDragDropTarget();
        }
        if (node_open && static_cast<int>(Nodes[i].ChildIDs.size()) != 0)
        {
            std::vector<Node*> Childs(Nodes[i].ChildIDs.size());
            for (int j = 0; j < static_cast<int>(Nodes.size()); j++)
                for (int x = 0; x < static_cast<int>(Nodes[i].ChildIDs.size()); x++)
                    if (Nodes[j].ID == Nodes[i].ChildIDs[x])
                        Childs[x] = &Nodes[j];

            CreateChildTreeNodes(Nodes, Childs, SelectedNode, i);

            ImGui::TreePop();
        }

        ImGui::InvisibleButton("UnderNode", ImVec2(ImGui::GetWindowSize().x, 5));
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("SCENEVIEW_TREENODE"))
            {
                int drag_index = *(int*)p->Data;

                if (drag_index != i)
                {
                    Nodes[i].ChildIDs.push_back(Nodes[drag_index].ID);

                    for (int j = 0; j < static_cast<int>(Nodes.size()); j++)
                        if (Nodes[j].ID == Nodes[drag_index].ParentID)
                            Nodes[j].ChildIDs.erase(std::find(Nodes[j].ChildIDs.begin(), Nodes[j].ChildIDs.end(), Nodes[drag_index].ID));

                    Nodes[drag_index].ParentID = Nodes[i].ID;
                    Nodes[drag_index].IsChild = true;
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::EndGroup();
        ImGui::PopID();
    }
    
    ImGui::EndChild();

    if (SelectedNode != -1)
    {
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << SelectedNode);
        else 
            selection_mask = (1 << SelectedNode);
    }
}

int SceneView::CreateChildTreeNodes(std::vector<Node>& Nodes, std::vector<Node*>& ChildNodes, int& SelectedNode, int CurrentParentIndex)
{
    static int selection_mask = (1 << 2);

    ImGui::BeginChild("NodeTree", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);

    for (int i = 0; i < static_cast<int>(ChildNodes.size()); i++)
    {
        ImGui::PushID(i + 1);
        ImGui::BeginGroup();

        int node_index = std::find(Nodes.begin(), Nodes.end(), ChildNodes[i]) - Nodes.begin();

        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        const bool is_selected = (selection_mask & (1 << node_index)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;

        if (static_cast<int>(ChildNodes[i]->ChildIDs.size()) == 0)
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool node_open = ImGui::TreeNodeEx((void*)(int*)&i, node_flags, ChildNodes[i]->Name.c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            SelectedNode = node_index;

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("SCENEVIEW_TREENODE", &SelectedNode, sizeof(SelectedNode), ImGuiCond_Once);
            ImGui::Text("This is a drag and drop source");
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("SCENEVIEW_TREENODE"))
            {
                int drag_index = *(int*)p->Data;

                if (drag_index != i)
                {
                    
                    if (!Nodes[drag_index].IsChild)
                    {
                        Nodes[drag_index].ParentID = ChildNodes[i]->ParentID;
                        Nodes[CurrentParentIndex].ChildIDs.push_back(Nodes[drag_index].ID);
                        Nodes[drag_index].IsChild = true;
                    }
                    else
                    {
                        bool pop_tree = ChildNodes[i]->ChildIDs.size() > 0 && ChildNodes[i]->TreeOpen;
                        if (Nodes[drag_index].ParentID == Nodes[node_index].ParentID)
                        {
                            int from = std::find(Nodes[CurrentParentIndex].ChildIDs.begin(), Nodes[CurrentParentIndex].ChildIDs.end(), Nodes[drag_index].ID) - Nodes[CurrentParentIndex].ChildIDs.begin();
                            int to = i;

                            if (from < to)
                                std::rotate(Nodes[CurrentParentIndex].ChildIDs.begin() + from, Nodes[CurrentParentIndex].ChildIDs.begin() + from + 1, Nodes[CurrentParentIndex].ChildIDs.begin() + to + 1);
                            else
                                std::rotate(Nodes[CurrentParentIndex].ChildIDs.begin() + to, Nodes[CurrentParentIndex].ChildIDs.begin() + from, Nodes[CurrentParentIndex].ChildIDs.begin() + from + 1);    
                        }
                        else
                        {
                            for (int j = 0; j < static_cast<int>(Nodes.size()); j++)
                            {
                                if (Nodes[j].ID == Nodes[drag_index].ParentID)
                                {
                                    Nodes[j].ChildIDs.erase(std::find(Nodes[j].ChildIDs.begin(), Nodes[j].ChildIDs.end(), Nodes[drag_index].ID));
                                }
                            }

                            Nodes[drag_index].ParentID = ChildNodes[i]->ParentID;
                            Nodes[CurrentParentIndex].ChildIDs.push_back(Nodes[drag_index].ID);
                            Nodes[drag_index].IsChild = true;
                        }


                        ImGui::EndGroup();
                        ImGui::PopID();
                        if (pop_tree)
                            ImGui::TreePop();
                        continue;
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }
        ChildNodes[i]->TreeOpen = node_open && static_cast<int>(ChildNodes[i]->ChildIDs.size()) != 0;
        if (node_open && static_cast<int>(ChildNodes[i]->ChildIDs.size()) != 0)
        {
            std::vector<Node*> Childs(Nodes[node_index].ChildIDs.size());
            for (int j = 0; j < static_cast<int>(Nodes.size()); j++)
                for (int x = 0; x < static_cast<int>(Nodes[node_index].ChildIDs.size()); x++)
                    if (Nodes[j].ID == Nodes[node_index].ChildIDs[x])
                        Childs[x] = &Nodes[j];

            if (CreateChildTreeNodes(Nodes, Childs, SelectedNode, node_index) == 1)
            {
                ImGui::TreePop();
                return 1;
            }

            ImGui::TreePop();
        }
        ImGui::InvisibleButton("UnderNode", ImVec2(ImGui::GetWindowSize().x, 5));
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("SCENEVIEW_TREENODE"))
            {
                int drag_index = *(int*)p->Data;

                if (drag_index != i)
                {
                    if (Nodes[drag_index].IsChild)
                        for (int j = 0; j < static_cast<int>(Nodes.size()); j++)
                            if (Nodes[j].ID == Nodes[drag_index].ParentID)
                                Nodes[j].ChildIDs.erase(std::find(Nodes[j].ChildIDs.begin(), Nodes[j].ChildIDs.end(), Nodes[drag_index].ID));

                    Nodes[drag_index].IsChild = true;
                    Nodes[node_index].ChildIDs.push_back(Nodes[drag_index].ID);
                    Nodes[drag_index].ParentID = Nodes[node_index].ID;
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::EndGroup();
        ImGui::PopID();
    }
    ImGui::EndChild();

    if (SelectedNode != -1)
    {
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << SelectedNode);
        else 
            selection_mask = (1 << SelectedNode);
    }

    return 0;
}