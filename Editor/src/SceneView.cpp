#include "SceneView.h"
#include "imgui.h"
#include "App.h"

SceneView::SceneView() {

}

SceneView::~SceneView() {

}

void SceneView::SceneViewSpace(const std::vector<Node>& nodes, int& selectednode) {
    ImGui::Begin("Scene");


    CreateTreeNodes(nodes, selectednode);

    
    ImGui::End();
}

void SceneView::CreateTreeNodes(const std::vector<Node>& nodes, int& selectednode, int startpoint, int length, bool isnodeschild) {
    static int selection_mask = (1 << 2);

    size_t looplength = nodes.size();

    size_t loopstart = 0;
    
    if (length != -1)
        looplength = length;
    if (startpoint != 0)
        loopstart = startpoint;

    for (size_t i = loopstart; i < (size_t)(loopstart + looplength); i++) {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;

        if (nodes[i].ChildCount == 0)
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool node_open = false;

        if (!nodes[i].IsChild || isnodeschild)
            node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, nodes[i].Name.c_str());
        
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            selectednode = i;
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
            ImGui::Text("This is a drag and drop source");
            ImGui::EndDragDropSource();
        }
        if (node_open && nodes[i].ChildCount != 0) {
            CreateTreeNodes(nodes, selectednode, i - nodes[i].ChildCount, nodes[i].ChildCount, true);
            ImGui::TreePop();
        }
        
    }
    if (selectednode != -1) {
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << selectednode);
        else 
            selection_mask = (1 << selectednode);
    }
}