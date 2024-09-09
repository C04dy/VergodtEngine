#include "SceneView.h"
#include "imgui.h"
#include "App.h"

SceneView::SceneView() {

}

SceneView::~SceneView() {

}

void SceneView::SceneViewSpace(const std::vector<Node>& Nodes, int& SelectedNode)
{
    ImGui::Begin("Scene");

    CreateTreeNodes(Nodes, SelectedNode);
    
    ImGui::End();
}

void SceneView::CreateTreeNodes(const std::vector<Node>& Nodes, int& SelectedNode, int StartPoint, int Length, bool IsNodesChild)
{
    static int selection_mask = (1 << 2);

    int loop_length = static_cast<int>(Nodes.size());

    int loop_start = 0;
    
    if (Length != -1)
        loop_length = Length;
    if (StartPoint != 0)
        loop_start = StartPoint;

    ImGui::BeginChild("NodeTree", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);
        
    for (int i = loop_start; i < loop_start + loop_length; i++)
    {
        ImGui::PushID(i + 1);
        ImGui::BeginGroup();

        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;

        if (Nodes[i].ChildCount == 0)
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool node_open = false;

        if (!Nodes[i].IsChild || IsNodesChild)
            node_open = ImGui::TreeNodeEx((void*)(int*)&i, node_flags, Nodes[i].Name.c_str());
        
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            SelectedNode = i;

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("SCENEVIEW_TREENODE", &SelectedNode, sizeof(SelectedNode), ImGuiCond_Once);
            ImGui::Text("This is a drag and drop source");
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("SCENEVIEW_TREENODE"))
                std::cout << *(int*)p->Data << " " << i << '\n';
            ImGui::EndDragDropTarget();
        }

        if (node_open && Nodes[i].ChildCount != 0)
        {
            CreateTreeNodes(Nodes, SelectedNode, i - Nodes[i].ChildCount, Nodes[i].ChildCount, true);
            ImGui::TreePop();
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