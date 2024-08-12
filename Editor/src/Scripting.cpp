#include "Scripting.h"
#include <math.h>
#include "imgui.h"
#include <string>

Scripting::Scripting() {

}

void Scripting::ScriptingSpace() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Script"))
    {
        ImGui::End();
        return;
    }

    // Draw a list of nodes on the left side
    bool open_context_menu = false;
    int node_hovered_in_list = -1;
    int node_hovered_in_scene = -1;
    ImGui::BeginChild("node_list", ImVec2(100, 0));
    ImGui::Text("Nodes");
    ImGui::Separator();
    for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
    {
        ScriptingNode* node = &nodes[node_idx];
        ImGui::PushID(node->ID);
        if (ImGui::Selectable(node->Name, node->ID == node_selected))
            node_selected = node->ID;
        if (ImGui::IsItemHovered())
        {
            node_hovered_in_list = node->ID;
            open_context_menu |= ImGui::IsMouseClicked(1);
        }
        ImGui::PopID();
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginGroup();

    const float NODE_SLOT_RADIUS = 4.0f;
    const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

    // Create our child canvas
    ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    ImGui::Checkbox("Show grid", &show_grid);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PopStyleVar(); // WindowPadding
    ImGui::PushItemWidth(120.0f);

    ImVec2 offset;
    offset.x = ImGui::GetCursorScreenPos().x + scrolling.x;
    offset.y = ImGui::GetCursorScreenPos().y + scrolling.y;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Display grid
    if (show_grid)
    {
        ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
        float GRID_SZ = 64.0f;
        ImVec2 win_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_sz = ImGui::GetWindowSize();
        for (float x = fmodf(scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
            draw_list->AddLine(ImVec2(x + win_pos.x, win_pos.y), ImVec2(x + win_pos.x, canvas_sz.y + win_pos.y), GRID_COLOR);
        for (float y = fmodf(scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
            draw_list->AddLine(ImVec2(win_pos.x, y + win_pos.y), ImVec2(canvas_sz.x + win_pos.x, y + win_pos.y), GRID_COLOR);
    }

    // Display links
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(0); // Background
    if (SelectedConnectionNode != -1) {
        if (NodeInputSelected != -1) {
            ImVec2 p1;
            p1.x = offset.x + nodes[SelectedConnectionNode].GetInputSlotPos(NodeInputSelected).x;
            p1.y = offset.y + nodes[SelectedConnectionNode].GetInputSlotPos(NodeInputSelected).y;
            draw_list->AddBezierCubic(p1, ImVec2( p1.x , p1.y), ImVec2(io.MousePos.x, io.MousePos.y), ImVec2(io.MousePos.x, io.MousePos.y), IM_COL32(200, 200, 100, 255), 3.0f);
        }
        if (NodeOutputSelected != -1) {
            ImVec2 p1;
            p1.x = offset.x + nodes[SelectedConnectionNode].GetOutputSlotPos(NodeOutputSelected).x;
            p1.y = offset.y + nodes[SelectedConnectionNode].GetOutputSlotPos(NodeOutputSelected).y;
            draw_list->AddBezierCubic(p1, ImVec2( p1.x , p1.y), ImVec2(io.MousePos.x, io.MousePos.y), ImVec2(io.MousePos.x, io.MousePos.y), IM_COL32(200, 200, 100, 255), 3.0f);
        }
    }
    for (int link_idx = 0; link_idx < links.Size; link_idx++)
    {
        NodeLink* link = &links[link_idx];
        ScriptingNode* node_inp = &nodes[link->InputIdx];
        ScriptingNode* node_out = &nodes[link->OutputIdx];
        ImVec2 p1;
        p1.x = offset.x + node_inp->GetOutputSlotPos(link->InputSlot).x;
        p1.y = offset.y + node_inp->GetOutputSlotPos(link->InputSlot).y;
        ImVec2 p2;
        p2.x = offset.x + node_out->GetInputSlotPos(link->OutputSlot).x;
        p2.y = offset.y + node_out->GetInputSlotPos(link->OutputSlot).y;
        draw_list->AddBezierCubic(p1, ImVec2( p1.x + 50, p1.y), ImVec2(p2.x + -50, p2.y), p2, IM_COL32(200, 200, 100, 255), 3.0f);
    }

    // Display nodes
    for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
    {
        ScriptingNode* node = &nodes[node_idx];
        ImGui::PushID(node->ID);
        ImVec2 node_rect_min;
        node_rect_min.x = offset.x + node->Pos.x;
        node_rect_min.y = offset.y + node->Pos.y;

        // Display node contents first
        draw_list->ChannelsSetCurrent(1); // Foreground
        bool old_any_active = ImGui::IsAnyItemActive();
        ImGui::SetCursorScreenPos(ImVec2(node_rect_min.x + NODE_WINDOW_PADDING.x, node_rect_min.y + NODE_WINDOW_PADDING.y));
        ImGui::BeginGroup(); // Lock horizontal position
        ImGui::Text("%s", node->Name);
        //ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
        //ImGui::ColorEdit3("##color", &node->Color.x);
        ImGui::EndGroup();

        // Save the size of what we have emitted and whether any of the widgets are being used
        bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
        node->Size.x = ImGui::GetItemRectSize().x + NODE_WINDOW_PADDING.x + NODE_WINDOW_PADDING.x;
        node->Size.y = ImGui::GetItemRectSize().y + NODE_WINDOW_PADDING.y + NODE_WINDOW_PADDING.y;
        ImVec2 node_rect_max;
        node_rect_max.x = node_rect_min.x + node->Size.x;
        node_rect_max.y = node_rect_min.y + node->Size.y;

        // Display node box
        draw_list->ChannelsSetCurrent(0); // Background
        ImGui::SetCursorScreenPos(node_rect_min);
        ImGui::InvisibleButton("node", node->Size);
        if (ImGui::IsItemHovered())
        {
            node_hovered_in_scene = node->ID;
            open_context_menu |= ImGui::IsMouseClicked(1);
        }
        bool node_moving_active = ImGui::IsItemActive();
        if (node_widgets_active || node_moving_active)
            node_selected = node->ID;
        if (node_moving_active && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            node->Pos.x = node->Pos.x + io.MouseDelta.x;
            node->Pos.y = node->Pos.y + io.MouseDelta.y;
        }

        ImU32 node_bg_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);
        draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
        draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
        for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++) {
            ImGui::SetCursorScreenPos(ImVec2(offset.x + node->GetInputSlotPos(slot_idx).x - NODE_SLOT_RADIUS, offset.y + node->GetInputSlotPos(slot_idx).y - NODE_SLOT_RADIUS));
            ImGui::InvisibleButton("hi", ImVec2(NODE_SLOT_RADIUS * 2, NODE_SLOT_RADIUS * 2));
            if (ImGui::IsItemHovered()) {
                if (ImGui::IsMouseClicked(0)) {
                    if (NodeOutputSelected != -1) {
                        links.push_back(NodeLink(SelectedConnectionNode, NodeOutputSelected, node_idx, slot_idx));
                        NodeOutputSelected = -1;
                        SelectedConnectionNode = -1;
                    } else {
                        NodeInputSelected = slot_idx;
                        SelectedConnectionNode = node_idx;
                    }
                }
            }

            draw_list->AddCircleFilled(ImVec2(offset.x + node->GetInputSlotPos(slot_idx).x, offset.y + node->GetInputSlotPos(slot_idx).y), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
        }
        for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++) {
            ImGui::SetCursorScreenPos(ImVec2(offset.x + node->GetOutputSlotPos(slot_idx).x - NODE_SLOT_RADIUS, offset.y + node->GetOutputSlotPos(slot_idx).y - NODE_SLOT_RADIUS));
            ImGui::InvisibleButton("hi", ImVec2(NODE_SLOT_RADIUS * 2, NODE_SLOT_RADIUS * 2));
            if (ImGui::IsItemHovered()) {
                if (ImGui::IsMouseClicked(0)) {
                    if (NodeInputSelected != -1) {
                        links.push_back(NodeLink(node_idx, slot_idx, SelectedConnectionNode, NodeInputSelected));
                        NodeInputSelected = -1;
                        SelectedConnectionNode = -1;
                    } else {
                        NodeOutputSelected = slot_idx;
                        SelectedConnectionNode = node_idx;
                    }
                }
            }

            draw_list->AddCircleFilled(ImVec2(offset.x + node->GetOutputSlotPos(slot_idx).x, offset.y + node->GetOutputSlotPos(slot_idx).y), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
        }

        ImGui::PopID();
    }
    draw_list->ChannelsMerge();

    // Open context menu
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) || !ImGui::IsAnyItemHovered())
        {
            node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
            open_context_menu = true;
        }
    if (open_context_menu)
    {
        NodeInputSelected = -1;
        NodeOutputSelected = -1;
        SelectedConnectionNode = -1;

        ImGui::OpenPopup("context_menu");
        if (node_hovered_in_list != -1)
            node_selected = node_hovered_in_list;
        if (node_hovered_in_scene != -1)
            node_selected = node_hovered_in_scene;
    }

    // Draw context menu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("context_menu")) {
        ScriptingNode* node = node_selected != -1 ? &nodes[node_selected] : NULL;
        ImVec2 scene_pos;
        scene_pos.x = ImGui::GetMousePosOnOpeningCurrentPopup().x - offset.x;
        scene_pos.y = ImGui::GetMousePosOnOpeningCurrentPopup().y - offset.y;
        if (node) {
            ImGui::Text("Node '%s'", node->Name);
            ImGui::Separator();
            if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
            if (ImGui::MenuItem("Delete", NULL, false, false)) {}
            if (ImGui::MenuItem("Copy", NULL, false, false)) {}
        } else {
            if (ImGui::MenuItem("Add")) { ImGui::OpenPopup }
            if (ImGui::MenuItem("Paste", NULL, false, false)) {}
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("Nodes")) {
        //ScriptingNode* node = node_selected != -1 ? &nodes[node_selected] : NULL;
        ImVec2 scene_pos;
        scene_pos.x = ImGui::GetMousePosOnOpeningCurrentPopup().x - offset.x;
        scene_pos.y = ImGui::GetMousePosOnOpeningCurrentPopup().y - offset.y;
        if (ImGui::MenuItem("Add")) { nodes.push_back(ScriptingNode(nodes.Size, scene_pos)); }
        if (ImGui::MenuItem("Paste", NULL, false, false)) {}
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

    // Scrolling
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f)) {
        scrolling.x = scrolling.x + io.MouseDelta.x;
        scrolling.y = scrolling.y + io.MouseDelta.y;
    }

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::EndGroup();

    ImGui::End();
}