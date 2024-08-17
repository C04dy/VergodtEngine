#include "Scripting.h"
#include <iostream>
#include <math.h>
#include "imgui.h"
#include "imgui_stdlib.h"
#include <string>
#include <fstream>
#include "StringFunctions.h"

void CreateKeyboardInputNode(std::vector<Scripting::ScriptingNode>& Nodes, ImVec2 Position, int key, int type) {
    Nodes.push_back(Scripting::ScriptingNode(Nodes.size(), Position, "KEYBOARDINPUT", 0, 1, "Keyboard Input"));
        
    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValueType::INT, new int(key), Nodes.size(), "Key"));
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("None");
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("A");

    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValueType::INT, new int(type), Nodes.size(), "Input Type"));
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("None");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is Key Just Pressed");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is Key Just Released");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is Key Pressed");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is Key Not Pressed");
}

void CreateMouseInputNode(std::vector<Scripting::ScriptingNode>& Nodes, ImVec2 Position, int key, int type) {
    Nodes.push_back(Scripting::ScriptingNode(Nodes.size(), Position, "MOUSEINPUT", 0, 1, "Mouse Input"));

    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValueType::INT, new int(key), Nodes.size(), "Key"));
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("None");
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("Mouse Button Left");
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("Mouse Button Middle");
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("Mouse Button Right");

    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValueType::INT, new int(type), Nodes.size(), "Input Type"));
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("None");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is MouseKey Just Pressed");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is MouseKey Just Released");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is MouseKey Pressed");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is MouseKey Not Pressed");
}

void CreatePrintNode(std::vector<Scripting::ScriptingNode>& Nodes, ImVec2 Position, const std::string& Msg) {
    Nodes.push_back(Scripting::ScriptingNode(Nodes.size(), Position, "PRINT", 1, 1, "Print"));
    
    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValueType::STRING, Msg, Nodes.size(), "Message"));
}

Scripting::Scripting() {
    ChangeScript("../../Assets/test.verscript");
}

void Scripting::SaveScript(const std::string& ScriptPath) {
    std::string Line;

    for (size_t i = 0; i < nodes.size(); i++) {
        Line += "[TYPE=";
        Line += nodes[i].Type;
        Line += "] ";

        if (IsLineExist(nodes[i].Type, "PRINT")) {
            Line += "(";
            Line += (char*)nodes[i].NodeValues[0].GetValue();
            Line += ") ";
        } else if (IsLineExist(nodes[i].Type, "KEYBOARDINPUT")) {
            Line += "[INPUTTYPE=";
            Line += RemoveSpaceAndUpperCase(nodes[i].NodeValues[1].ComboItems[*(int*)nodes[i].NodeValues[1].GetValue()]);
            Line += "] ";
            
            Line += "[KEY=";
            Line += nodes[i].NodeValues[0].ComboItems[*(int*)nodes[i].NodeValues[0].GetValue()];
            Line += "] ";
        } else if (IsLineExist(nodes[i].Type, "MOUSEINPUT")) {
            Line += "[INPUTTYPE=";
            Line += RemoveSpaceAndUpperCase(nodes[i].NodeValues[1].ComboItems[*(int*)nodes[i].NodeValues[1].GetValue()]);
            Line += "] ";
            
            Line += "[KEY=";
            Line += std::to_string(*(int*)nodes[i].NodeValues[0].GetValue());
            Line += "] ";
        }

        for (size_t j = 0; j < links.size(); j++) {
            if (links[j].OutputIdx == nodes[i].ID) {
                Line += "[CONNECTEDID=";
                Line += std::to_string(links[j].InputIdx);
                Line += "] ";
            }
        }
        

        Line += "(X=" + std::to_string(nodes[i].Pos.x) + ") ";
        Line += "(Y=" + std::to_string(nodes[i].Pos.y) + ") ";
        Line += '\n';
    }
    Line.erase(Line.size() - 2, Line.size());

    std::ofstream WriteFile(ScriptPath);
    WriteFile << Line;
}

void Scripting::ChangeScript(const std::string& ScriptPath) {
    std::string Line;
    std::ifstream ScriptFile(ScriptPath);

    if (ScriptFile.fail()) {
        std::cout << "Script File did not found.\n";
        return;
    }
    
    while (std::getline(ScriptFile, Line)) {
        std::string node_type = GetLineBetween(Line, "[TYPE=", "]");
        ImVec2 scene_pos = ImVec2(std::stof(GetLineBetween(Line, "(X=", ")")), std::stof(GetLineBetween(Line, "(Y=", ")")));

        if (node_type == "START") {
            nodes.push_back(ScriptingNode(nodes.size(), scene_pos, node_type.c_str(), 0, 1, "Start"));
        } else if(node_type == "UPDATE") {
            nodes.push_back(ScriptingNode(nodes.size(), scene_pos, node_type.c_str(), 0, 1, "Update"));
        } else if(node_type == "PRINT") {
            CreatePrintNode(nodes, scene_pos, GetLineBetween(Line, "(", ")"));
        } else if (node_type == "KEYBOARDINPUT") {
            CreateKeyboardInputNode(nodes, scene_pos, KeyToInt(GetLineBetween(Line, "[KEY=", "]")), InputTypeToInt(GetLineBetween(Line, "[INPUTTYPE=", "]")));
        } else if (node_type == "MOUSEINPUT") {
            CreateMouseInputNode(nodes, scene_pos, std::stoi(GetLineBetween(Line, "[KEY=", "]")), InputTypeToInt(GetLineBetween(Line, "[INPUTTYPE=", "]")));
        }

        if (IsLineExist(Line, "CONNECTEDID")) {
            int connectedNode = std::stoi(GetLineBetween(Line, "[CONNECTEDID=", "]"));
            
            links.push_back(NodeLink(connectedNode, 0, nodes.size() - 1, 0));
        }
    }
    ScriptFile.close();

    CurrentScriptPath = ScriptPath;
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
    for (size_t node_idx = 0; node_idx < nodes.size(); node_idx++) {
        ImGui::PushID(nodes[node_idx].ID);
        if (ImGui::Selectable(nodes[node_idx].Name, nodes[node_idx].ID == node_selected))
            node_selected = nodes[node_idx].ID;
        if (ImGui::IsItemHovered())
        {
            node_hovered_in_list = nodes[node_idx].ID;
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
    for (size_t link_idx = 0; link_idx < links.size(); link_idx++)
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
    for (size_t node_idx = 0; node_idx < nodes.size(); node_idx++)
    {
        ImGui::PushID(nodes[node_idx].ID);
        ImVec2 node_rect_min;
        node_rect_min.x = offset.x + nodes[node_idx].Pos.x;
        node_rect_min.y = offset.y + nodes[node_idx].Pos.y;

        // Display node contents first
        draw_list->ChannelsSetCurrent(1); // Foreground
        bool old_any_active = ImGui::IsAnyItemActive();
        ImGui::SetCursorScreenPos(ImVec2(node_rect_min.x + NODE_WINDOW_PADDING.x, node_rect_min.y + NODE_WINDOW_PADDING.y));
        ImGui::BeginGroup(); // Lock horizontal position
        ImGui::Text("%s", nodes[node_idx].Name);
        for (size_t i = 0; i < nodes[node_idx].NodeValues.size(); i++) {
            char* valuename = nodes[node_idx].NodeValues[i].ValueName.data();
            switch (nodes[node_idx].NodeValues[i].Type) {
            case ScriptingNodeValueType::NULLTYPE:
            case ScriptingNodeValueType::FLOAT:
                ImGui::InputFloat(valuename, ((float*)nodes[node_idx].NodeValues[i].GetValue()));
                break;
            case ScriptingNodeValueType::INT: {
                int intval = *((int*)nodes[node_idx].NodeValues[i].GetValue());

                if (nodes[node_idx].NodeValues[i].ComboItems.size() != 0) {
                    if (ImGui::BeginCombo(valuename, nodes[node_idx].NodeValues[i].ComboItems[intval].c_str() )) {
                        for (size_t j = 0; j < nodes[node_idx].NodeValues[i].ComboItems.size(); ++j) {
                            const bool isSelected = (intval == (int)j);
                            if (ImGui::Selectable(nodes[node_idx].NodeValues[i].ComboItems[j].c_str(), isSelected)) {
                                intval = j;
                                nodes[node_idx].NodeValues[i].SetValue(new int(intval));
                            }
                            if (isSelected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }
                } else {
                    if (ImGui::InputInt(valuename, &intval))
                        nodes[node_idx].NodeValues[i].SetValue(new int(intval));
                }
            }   break;
            case ScriptingNodeValueType::STRING:
                std::string msg = (char*)nodes[node_idx].NodeValues[i].GetValue();
                if (ImGui::InputText(valuename, &msg))
                    nodes[node_idx].NodeValues[i].SetValue(msg);
                break;
            }
        }
        ImGui::EndGroup();

        // Save the size of what we have emitted and whether any of the widgets are being used
        bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
        nodes[node_idx].Size.x = ImGui::GetItemRectSize().x + NODE_WINDOW_PADDING.x + NODE_WINDOW_PADDING.x;
        nodes[node_idx].Size.y = ImGui::GetItemRectSize().y + NODE_WINDOW_PADDING.y + NODE_WINDOW_PADDING.y;
        ImVec2 node_rect_max;
        node_rect_max.x = node_rect_min.x + nodes[node_idx].Size.x;
        node_rect_max.y = node_rect_min.y + nodes[node_idx].Size.y;

        // Display node box
        draw_list->ChannelsSetCurrent(0); // Background
        ImGui::SetCursorScreenPos(node_rect_min);
        ImGui::InvisibleButton("node", nodes[node_idx].Size);
        if (ImGui::IsItemHovered())
        {
            node_hovered_in_scene = nodes[node_idx].ID;
            open_context_menu |= ImGui::IsMouseClicked(1);
        }
        bool node_moving_active = ImGui::IsItemActive();
        if (node_widgets_active || node_moving_active)
            node_selected = nodes[node_idx].ID;
        if (node_moving_active && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            nodes[node_idx].Pos.x = nodes[node_idx].Pos.x + io.MouseDelta.x;
            nodes[node_idx].Pos.y = nodes[node_idx].Pos.y + io.MouseDelta.y;
        }

        ImU32 node_bg_color = (node_hovered_in_list == nodes[node_idx].ID || node_hovered_in_scene == nodes[node_idx].ID || (node_hovered_in_list == -1 && node_selected == nodes[node_idx].ID)) ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);
        draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
        draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
        
        for (int slot_idx = 0; slot_idx < nodes[node_idx].InputsCount; slot_idx++) {
            ImGui::SetCursorScreenPos(ImVec2(offset.x + nodes[node_idx].GetInputSlotPos(slot_idx).x - NODE_SLOT_RADIUS, offset.y + nodes[node_idx].GetInputSlotPos(slot_idx).y - NODE_SLOT_RADIUS));
            ImGui::InvisibleButton("inputs", ImVec2(NODE_SLOT_RADIUS * 2, NODE_SLOT_RADIUS * 2));
            if (ImGui::IsItemHovered()) {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    if (NodeOutputSelected != -1) {
                        links.push_back(NodeLink(SelectedConnectionNode, NodeOutputSelected, node_idx, slot_idx));
                        NodeOutputSelected = -1;
                        NodeInputSelected = -1;
                        SelectedConnectionNode = -1;
                    } else {
                        NodeInputSelected = slot_idx;
                        SelectedConnectionNode = node_idx;
                    }
                }
            }
            draw_list->AddCircleFilled(ImVec2(offset.x + nodes[node_idx].GetInputSlotPos(slot_idx).x, offset.y + nodes[node_idx].GetInputSlotPos(slot_idx).y), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
        }
        for (int slot_idx = 0; slot_idx < nodes[node_idx].OutputsCount; slot_idx++) {
            ImGui::SetCursorScreenPos(ImVec2(offset.x + nodes[node_idx].GetOutputSlotPos(slot_idx).x - NODE_SLOT_RADIUS, offset.y + nodes[node_idx].GetOutputSlotPos(slot_idx).y - NODE_SLOT_RADIUS));
            ImGui::InvisibleButton("output", ImVec2(NODE_SLOT_RADIUS * 2, NODE_SLOT_RADIUS * 2));
            if (ImGui::IsItemHovered()) {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    if (NodeInputSelected != -1) {
                        links.push_back(NodeLink(node_idx, slot_idx, SelectedConnectionNode, NodeInputSelected));
                        NodeOutputSelected = -1;
                        NodeInputSelected = -1;
                        SelectedConnectionNode = -1;
                    } else {
                        NodeOutputSelected = slot_idx;
                        SelectedConnectionNode = node_idx;
                    }
                }
            }

            draw_list->AddCircleFilled(ImVec2(offset.x + nodes[node_idx].GetOutputSlotPos(slot_idx).x, offset.y + nodes[node_idx].GetOutputSlotPos(slot_idx).y), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
        }
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
            NodeInputSelected = -1;
            NodeOutputSelected = -1;
            SelectedConnectionNode = -1;
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
    bool OpenNodeMenu = false;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("Node Menu")) {
        ImVec2 scene_pos;
        scene_pos.x = ImGui::GetMousePosOnOpeningCurrentPopup().x - offset.x;
        scene_pos.y = ImGui::GetMousePosOnOpeningCurrentPopup().y - offset.y;
        if (ImGui::MenuItem("Start")) { nodes.push_back(ScriptingNode(nodes.size(), scene_pos, "START", 0, 1, "Start")); }
        if (ImGui::MenuItem("Update")) { nodes.push_back(ScriptingNode(nodes.size(), scene_pos, "UPDATE", 0, 1, "Update")); }
        if (ImGui::MenuItem("Print")) { CreatePrintNode(nodes, scene_pos, ""); }
        if (ImGui::MenuItem("Keyboard Input")) { CreateKeyboardInputNode(nodes, scene_pos, 0, 0); }
        if (ImGui::MenuItem("Mouse Input")) { CreateMouseInputNode(nodes, scene_pos, 0, 0); }
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("context_menu")) {
        ScriptingNode* node = node_selected != -1 ? &nodes[node_selected] : NULL;
        if (node) {
            ImGui::Text("Node '%s'", node->Name);
            ImGui::Separator();
            if (ImGui::MenuItem("Copy", NULL, false, false)) {}
        } else {
            if (ImGui::MenuItem("Scripting Nodes")) { OpenNodeMenu = true; }
        }
        ImGui::EndPopup();
    }
    if (OpenNodeMenu) {
        ImGui::OpenPopup("Node Menu");
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

    if (ImGui::IsKeyPressed(ImGuiKey_LeftCtrl)) {
        if (ImGui::IsKeyDown(ImGuiKey_S)) {
            SaveScript(CurrentScriptPath);
        }
    }
}