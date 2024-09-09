#include "Scripting.h"
#include <math.h>
#include "imgui.h"
#include "imgui_stdlib.h"
#include <string>
#include <fstream>
#include "StringFunctions.h"

// Used this example as a base here since i started using ImGui for the first time and started to learn it from here
// https://gist.github.com/ocornut/7e9b3ec566a333d725d4

void CreateKeyboardInputNode(std::vector<Scripting::ScriptingNode>& Nodes, ImVec2 Position, int Key, int Type)
{
    Nodes.push_back(Scripting::ScriptingNode(Position, "KEYBOARDINPUT", 0, 1, "Keyboard Input"));

    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValue::Type::INT, new int(Key), "Key"));
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("None");
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("A");

    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValue::Type::INT, new int(Type), "Input Type"));
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("None");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is Key Just Pressed");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is Key Just Released");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is Key Pressed");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is Key Not Pressed");
}

void CreateMouseInputNode(std::vector<Scripting::ScriptingNode>& Nodes, ImVec2 Position, int Key, int Type)
{
    Nodes.push_back(Scripting::ScriptingNode(Position, "MOUSEINPUT", 0, 1, "Mouse Input"));

    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValue::Type::INT, new int(Key), "Key"));
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("None");
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("Mouse Button Left");
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("Mouse Button Middle");
    Nodes[Nodes.size() - 1].NodeValues[0].ComboItems.push_back("Mouse Button Right");

    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValue::Type::INT, new int(Type), "Input Type"));
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("None");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is MouseKey Just Pressed");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is MouseKey Just Released");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is MouseKey Pressed");
    Nodes[Nodes.size() - 1].NodeValues[1].ComboItems.push_back("Is MouseKey Not Pressed");
}

void CreatePrintNode(std::vector<Scripting::ScriptingNode>& Nodes, ImVec2 Position, const std::string& Message)
{
    Nodes.push_back(Scripting::ScriptingNode(Position, "PRINT", 1, 1, "Print"));
    
    Nodes[Nodes.size() - 1].NodeValues.push_back(Scripting::ScriptingNodeValue(Scripting::ScriptingNodeValue::Type::STRING, new std::string(Message), "Message"));
}

Scripting::Scripting()
{

}

void Scripting::SaveScript(const std::string& ScriptPath)
{
    std::string line;

    for (int i = 0; i < static_cast<int>(m_Scripts[m_CurrentScript].Nodes.size()); i++)
    {
        line += "[TYPE=";
        line += m_Scripts[m_CurrentScript].Nodes[i].Type;
        line += "] ";

        if (IsLineExist(m_Scripts[m_CurrentScript].Nodes[i].Type, "PRINT"))
        {
            line += "(";
            line += *(std::string*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[0].Value;
            line += ") ";
        }
        else if (IsLineExist(m_Scripts[m_CurrentScript].Nodes[i].Type, "KEYBOARDINPUT"))
        {
            line += "[INPUTTYPE=";
            line += RemoveSpaceAndUpperCase(m_Scripts[m_CurrentScript].Nodes[i].NodeValues[1].ComboItems[*(int*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[1].Value]);
            line += "] ";
            
            line += "[KEY=";
            line += m_Scripts[m_CurrentScript].Nodes[i].NodeValues[0].ComboItems[*(int*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[0].Value];
            line += "] ";
        }
        else if (IsLineExist(m_Scripts[m_CurrentScript].Nodes[i].Type, "MOUSEINPUT"))
        {
            line += "[INPUTTYPE=";
            line += RemoveSpaceAndUpperCase(m_Scripts[m_CurrentScript].Nodes[i].NodeValues[1].ComboItems[*(int*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[1].Value]);
            line += "] ";
            
            line += "[KEY=";
            line += std::to_string(*(int*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[0].Value);
            line += "] ";
        }
        else if (IsLineExist(m_Scripts[m_CurrentScript].Nodes[i].Type, "APPLYFORCE"))
        {
            line += "[FORCEX=";
            line += std::to_string(*(float*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[0].Value);
            line += "] ";
            
            line += "[FORCEY=";
            line += std::to_string(*(float*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[1].Value);
            line += "] ";
        }
        else if (IsLineExist(m_Scripts[m_CurrentScript].Nodes[i].Type, "SETVELOCITY"))
        {
            line += "[VELOCITYX=";
            line += std::to_string(*(float*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[0].Value);
            line += "] ";
            
            line += "[VELOCITYY=";
            line += std::to_string(*(float*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[1].Value);
            line += "] ";
        }

        for (int j = 0; j < static_cast<int>(m_Scripts[m_CurrentScript].Links.size()); j++)
        {
            if (m_Scripts[m_CurrentScript].Links[j].OutputIndex == i)
            {
                line += "[CONNECTEDID=";
                line += std::to_string(m_Scripts[m_CurrentScript].Links[j].InputIndex);
                line += "] ";
            }
        }
        
        line += "(X=" + std::to_string(m_Scripts[m_CurrentScript].Nodes[i].Position.x) + ") ";
        line += "(Y=" + std::to_string(m_Scripts[m_CurrentScript].Nodes[i].Position.y) + ") ";
        line += '\n';
    }
    line.erase(line.size() - 2, line.size());

    std::ofstream write_file(ScriptPath);
    write_file << line;

    m_Scripts[m_CurrentScript].Saved = true;
}

void Scripting::LoadScript(const std::string& ScriptPath)
{
    std::string line;
    std::ifstream script_file(ScriptPath);

    m_Scripts.push_back(Script());
    m_CurrentScript = m_Scripts.size() - 1;

    if (script_file.fail())
    {
        std::cout << "Script File did not found.\n";
        return;
    }
    
    while (std::getline(script_file, line))
    {
        std::string node_type = GetLineBetween(line, "[TYPE=", "]");
        ImVec2 scene_pos = ImVec2(std::stof(GetLineBetween(line, "(X=", ")")), std::stof(GetLineBetween(line, "(Y=", ")")));

        if (node_type == "START")
        {
            m_Scripts[m_CurrentScript].Nodes.push_back(ScriptingNode(scene_pos, node_type.c_str(), 0, 1, "Start"));
        }
        else if (node_type == "UPDATE")
        {
            m_Scripts[m_CurrentScript].Nodes.push_back(ScriptingNode(scene_pos, node_type.c_str(), 0, 1, "Update"));
        }
        else if (node_type == "PRINT")
        {
            CreatePrintNode(m_Scripts[m_CurrentScript].Nodes, scene_pos, GetLineBetween(line, "(", ")"));
        }
        else if (node_type == "KEYBOARDINPUT")
        {
            CreateKeyboardInputNode(m_Scripts[m_CurrentScript].Nodes, scene_pos, KeyToInt(GetLineBetween(line, "[KEY=", "]")), InputTypeToInt(GetLineBetween(line, "[INPUTTYPE=", "]")));
        }
        else if (node_type == "MOUSEINPUT")
        {
            CreateMouseInputNode(m_Scripts[m_CurrentScript].Nodes, scene_pos, std::stoi(GetLineBetween(line, "[KEY=", "]")), InputTypeToInt(GetLineBetween(line, "[INPUTTYPE=", "]")));
        }
        else if (node_type == "APPLYFORCE")
        {
            m_Scripts[m_CurrentScript].Nodes.push_back(ScriptingNode(scene_pos, node_type.c_str(), 1, 1, "Apply Force"));

            float x = std::stof(GetLineBetween(line, "[FORCEX=", "]"));
            float y = std::stof(GetLineBetween(line, "[FORCEY=", "]"));
            
            m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].Nodes.size() - 1].NodeValues.push_back(ScriptingNodeValue(ScriptingNodeValue::Type::FLOAT, new float(x), "X"));
            m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].Nodes.size() - 1].NodeValues.push_back(ScriptingNodeValue(ScriptingNodeValue::Type::FLOAT, new float(y), "Y"));
        }
        else if (node_type == "SETVELOCITY")
        {
            m_Scripts[m_CurrentScript].Nodes.push_back(ScriptingNode(scene_pos, node_type.c_str(), 1, 1, "Set Velocity"));

            float x = std::stof(GetLineBetween(line, "[VELOCITYX=", "]"));
            float y = std::stof(GetLineBetween(line, "[VELOCITYY=", "]"));
            
            m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].Nodes.size() - 1].NodeValues.push_back(ScriptingNodeValue(ScriptingNodeValue::Type::FLOAT, new float(x), "X"));
            m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].Nodes.size() - 1].NodeValues.push_back(ScriptingNodeValue(ScriptingNodeValue::Type::FLOAT, new float(y), "Y"));
        }

        if (IsLineExist(line, "CONNECTEDID"))
        {
            int connectedNode = std::stoi(GetLineBetween(line, "[CONNECTEDID=", "]"));
            
            m_Scripts[m_CurrentScript].Links.push_back(NodeLink(connectedNode, 0, m_Scripts[m_CurrentScript].Nodes.size() - 1, 0));
        }
    }
    script_file.close();

    m_Scripts[m_CurrentScript].CurrentScriptPath = ScriptPath;
}

void Scripting::ScriptingSpace()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Scripting"))
    {
        ImGui::End();
        return;
    }

    ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_S, ImGuiInputFlags_Tooltip);
        if (ImGui::Button("Save") && m_Scripts.size() != 0)
            SaveScript(m_Scripts[m_CurrentScript].CurrentScriptPath);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        for (int i = 0; i < static_cast<int>(m_Scripts.size()); i++)
        {
            ImGuiTabItemFlags tabitemflags = ImGuiTabItemFlags_None;
            if (!m_Scripts[i].Saved)
                tabitemflags = ImGuiTabItemFlags_UnsavedDocument;
            if (ImGui::BeginTabItem(m_Scripts[i].CurrentScriptPath.c_str(), nullptr, tabitemflags))
            {
                m_CurrentScript = i;
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
    ImGui::Separator();

        // Draw a list of nodes on the left side
    bool open_context_menu = false;
    int node_hovered_in_list = -1;
    int node_hovered_in_scene = -1;
    ImGui::BeginChild("node_list", ImVec2(100, 0));
    ImGui::Text("Nodes");
    ImGui::Separator();
    if (m_Scripts.size() != 0)
    {
        for (int i = 0; i < static_cast<int>(m_Scripts[m_CurrentScript].Nodes.size()); i++)
        {
            ImGui::PushID(i);
            if (ImGui::Selectable(m_Scripts[m_CurrentScript].Nodes[i].Name, i == m_Scripts[m_CurrentScript].SelectedNode))
                m_Scripts[m_CurrentScript].SelectedNode = i;
            if (ImGui::IsItemHovered())
            {
                node_hovered_in_list = i;
                open_context_menu |= ImGui::IsMouseClicked(1);
            }
            ImGui::PopID();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginGroup();

    const float node_slot_radius = 4.0f;
    const ImVec2 node_window_padding(8.0f, 8.0f);

    // Create our child canvas
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PopStyleVar(); // WindowPadding
    ImGui::PushItemWidth(120.0f);

    ImVec2 offset;
    offset.x = ImGui::GetCursorScreenPos().x + m_Scrolling.x;
    offset.y = ImGui::GetCursorScreenPos().y + m_Scrolling.y;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Display grid
    if (m_ShowGrid)
    {
        ImU32 grid_color = IM_COL32(200, 200, 200, 40);
        float grid_size = 64.0f;
        ImVec2 window_position = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetWindowSize();
        for (float x = fmodf(m_Scrolling.x, grid_size); x < canvas_size.x; x += grid_size)
            draw_list->AddLine(ImVec2(x + window_position.x, window_position.y), ImVec2(x + window_position.x, canvas_size.y + window_position.y), grid_color);
        for (float y = fmodf(m_Scrolling.y, grid_size); y < canvas_size.y; y += grid_size)
            draw_list->AddLine(ImVec2(window_position.x, y + window_position.y), ImVec2(canvas_size.x + window_position.x, y + window_position.y), grid_color);
    }

    if (m_Scripts.size() != 0)
    {
        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(0);
        if (m_Scripts[m_CurrentScript].SelectedConnectionNode != -1)
        {
            if (m_Scripts[m_CurrentScript].NodeInputSelected != -1)
            {
                ImVec2 p1;
                p1.x = offset.x + m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedConnectionNode].GetInputSlotPos(m_Scripts[m_CurrentScript].NodeInputSelected).x;
                p1.y = offset.y + m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedConnectionNode].GetInputSlotPos(m_Scripts[m_CurrentScript].NodeInputSelected).y;
                draw_list->AddBezierCubic(p1, ImVec2(p1.x, p1.y), ImVec2(io.MousePos.x, io.MousePos.y), ImVec2(io.MousePos.x, io.MousePos.y), IM_COL32(200, 200, 100, 255), 3.0f);
            }
            if (m_Scripts[m_CurrentScript].NodeOutputSelected != -1)
            {
                ImVec2 p1;
                p1.x = offset.x + m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedConnectionNode].GetOutputSlotPos(m_Scripts[m_CurrentScript].NodeOutputSelected).x;
                p1.y = offset.y + m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedConnectionNode].GetOutputSlotPos(m_Scripts[m_CurrentScript].NodeOutputSelected).y;
                draw_list->AddBezierCubic(p1, ImVec2(p1.x, p1.y), ImVec2(io.MousePos.x, io.MousePos.y), ImVec2(io.MousePos.x, io.MousePos.y), IM_COL32(200, 200, 100, 255), 3.0f);
            }
        }
        for (NodeLink link : m_Scripts[m_CurrentScript].Links)
        {
            ScriptingNode* node_inp = &m_Scripts[m_CurrentScript].Nodes[link.InputIndex];
            ScriptingNode* node_out = &m_Scripts[m_CurrentScript].Nodes[link.OutputIndex];
            ImVec2 p1;
            p1.x = offset.x + node_inp->GetOutputSlotPos(link.InputSlot).x;
            p1.y = offset.y + node_inp->GetOutputSlotPos(link.InputSlot).y;
            ImVec2 p2;
            p2.x = offset.x + node_out->GetInputSlotPos(link.OutputSlot).x;
            p2.y = offset.y + node_out->GetInputSlotPos(link.OutputSlot).y;
            draw_list->AddBezierCubic(p1, ImVec2(p1.x + 50, p1.y), ImVec2(p2.x + -50, p2.y), p2, IM_COL32(200, 200, 100, 255), 3.0f);
        }


        // Display nodes
        for (int i = 0; i < static_cast<int>(m_Scripts[m_CurrentScript].Nodes.size()); i++)
        {
            ImGui::PushID(i);
            ImVec2 node_rect_min;
            node_rect_min.x = offset.x + m_Scripts[m_CurrentScript].Nodes[i].Position.x;
            node_rect_min.y = offset.y + m_Scripts[m_CurrentScript].Nodes[i].Position.y;

            // Display node contents first
            draw_list->ChannelsSetCurrent(1); // Foreground
            bool old_any_active = ImGui::IsAnyItemActive();
            ImGui::SetCursorScreenPos(ImVec2(node_rect_min.x + node_window_padding.x, node_rect_min.y + node_window_padding.y));
            ImGui::BeginGroup(); // Lock horizontal position
            ImGui::Text("%s", m_Scripts[m_CurrentScript].Nodes[i].Name);
            for (int j = 0; j < static_cast<int>(m_Scripts[m_CurrentScript].Nodes[i].NodeValues.size()); j++)
            {
                char* valuename = m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].ValueName.data();
                switch (m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].ValueType) {
                case ScriptingNodeValue::Type::FLOAT:
                    ImGui::InputFloat(valuename, ((float*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].Value));
                    break;
                case ScriptingNodeValue::Type::INT: {
                    int int_value = *((int*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].Value);

                    if (m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].ComboItems.size() != 0)
                    {
                        if (ImGui::BeginCombo(valuename, m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].ComboItems[int_value].c_str()))
                        {
                            for (int x = 0; x < static_cast<int>(m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].ComboItems.size()); x++)
                            {
                                const bool isSelected = (int_value == x);
                                if (ImGui::Selectable(m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].ComboItems[x].c_str(), isSelected))
                                {
                                    int_value = x;
                                    m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].Value = new int(int_value);
                                    m_Scripts[m_CurrentScript].Saved = false;
                                }
                                if (isSelected)
                                {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                            ImGui::EndCombo();
                        }
                    }
                    else
                    {
                        if (ImGui::InputInt(valuename, &int_value))
                        {
                            m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].Value = new int(int_value);
                            m_Scripts[m_CurrentScript].Saved = false;
                        }
                    }
                }   break;
                case ScriptingNodeValue::Type::STRING: {
                    std::string msg = *(std::string*)m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].Value;
                    if (ImGui::InputText(valuename, &msg)) {
                        m_Scripts[m_CurrentScript].Nodes[i].NodeValues[j].Value = new std::string(msg);
                        m_Scripts[m_CurrentScript].Saved = false;
                    }
                }    break;
                default:
                    break;
                }
            }
            ImGui::EndGroup();

            // Save the size of what we have emitted and whether any of the widgets are being used
            bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
            m_Scripts[m_CurrentScript].Nodes[i].Size.x = ImGui::GetItemRectSize().x + node_window_padding.x + node_window_padding.x;
            m_Scripts[m_CurrentScript].Nodes[i].Size.y = ImGui::GetItemRectSize().y + node_window_padding.y + node_window_padding.y;
            ImVec2 node_rect_max;
            node_rect_max.x = node_rect_min.x + m_Scripts[m_CurrentScript].Nodes[i].Size.x;
            node_rect_max.y = node_rect_min.y + m_Scripts[m_CurrentScript].Nodes[i].Size.y;

            // Display node box
            draw_list->ChannelsSetCurrent(0); // Background
            ImGui::SetCursorScreenPos(node_rect_min);
            ImGui::InvisibleButton("node", m_Scripts[m_CurrentScript].Nodes[i].Size);
            if (ImGui::IsItemHovered()) {
                node_hovered_in_scene = i;
                open_context_menu |= ImGui::IsMouseClicked(ImGuiMouseButton_Right);
            }
            bool node_moving_active = ImGui::IsItemActive();
            if (node_widgets_active || node_moving_active)
                m_Scripts[m_CurrentScript].SelectedNode = i;
            if (node_moving_active && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                m_Scripts[m_CurrentScript].Nodes[i].Position.x = m_Scripts[m_CurrentScript].Nodes[i].Position.x + io.MouseDelta.x;
                m_Scripts[m_CurrentScript].Nodes[i].Position.y = m_Scripts[m_CurrentScript].Nodes[i].Position.y + io.MouseDelta.y;
                m_Scripts[m_CurrentScript].Saved = false;
            }

            ImU32 node_bg_color = (node_hovered_in_list == i || node_hovered_in_scene == i || (node_hovered_in_list == -1 && m_Scripts[m_CurrentScript].SelectedNode == i)) ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);
            draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
            draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);

            for (int j = 0; j < m_Scripts[m_CurrentScript].Nodes[i].InputsCount; j++)
            {
                ImGui::SetCursorScreenPos(ImVec2(offset.x + m_Scripts[m_CurrentScript].Nodes[i].GetInputSlotPos(j).x - node_slot_radius, offset.y + m_Scripts[m_CurrentScript].Nodes[i].GetInputSlotPos(j).y - node_slot_radius));
                ImGui::InvisibleButton("inputs", ImVec2(node_slot_radius * 2, node_slot_radius * 2));
                if (ImGui::IsItemHovered())
                {
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        if (m_Scripts[m_CurrentScript].NodeOutputSelected != -1)
                        {
                            if (i < m_Scripts[m_CurrentScript].SelectedConnectionNode)
                            {
                                auto from = m_Scripts[m_CurrentScript].Nodes.begin() + i;
                                auto to = m_Scripts[m_CurrentScript].Nodes.begin() + m_Scripts[m_CurrentScript].SelectedConnectionNode;

                                std::rotate(from, from + 1, to + 1);

                                m_Scripts[m_CurrentScript].Links.push_back(NodeLink(m_Scripts[m_CurrentScript].SelectedConnectionNode - 1, m_Scripts[m_CurrentScript].NodeOutputSelected, m_Scripts[m_CurrentScript].SelectedConnectionNode, j));

                                for (int x = i; x < static_cast<int>(m_Scripts[m_CurrentScript].Links.size()); x++)
                                {
                                    m_Scripts[m_CurrentScript].Links[x].InputIndex = m_Scripts[m_CurrentScript].Links[x].InputIndex - 1;
                                    m_Scripts[m_CurrentScript].Links[x].OutputIndex = m_Scripts[m_CurrentScript].Links[x].OutputIndex - 1;
                                }

                                m_Scripts[m_CurrentScript].NodeOutputSelected = -1;
                                m_Scripts[m_CurrentScript].NodeInputSelected = -1;
                                m_Scripts[m_CurrentScript].SelectedConnectionNode = -1;
                                m_Scripts[m_CurrentScript].Saved = false;
                                break;
                            }

                            m_Scripts[m_CurrentScript].Links.push_back(NodeLink(m_Scripts[m_CurrentScript].SelectedConnectionNode, m_Scripts[m_CurrentScript].NodeOutputSelected, i, j));
                            m_Scripts[m_CurrentScript].NodeOutputSelected = -1;
                            m_Scripts[m_CurrentScript].NodeInputSelected = -1;
                            m_Scripts[m_CurrentScript].SelectedConnectionNode = -1;
                            m_Scripts[m_CurrentScript].Saved = false;
                        }
                        else
                        {
                            m_Scripts[m_CurrentScript].NodeInputSelected = j;
                            m_Scripts[m_CurrentScript].SelectedConnectionNode = i;
                            m_Scripts[m_CurrentScript].Saved = false;
                        }
                    }
                }
                draw_list->AddCircleFilled(ImVec2(offset.x + m_Scripts[m_CurrentScript].Nodes[i].GetInputSlotPos(j).x, offset.y + m_Scripts[m_CurrentScript].Nodes[i].GetInputSlotPos(j).y), node_slot_radius, IM_COL32(150, 150, 150, 150));
            }
            for (int j = 0; j < m_Scripts[m_CurrentScript].Nodes[i].OutputsCount; j++)
            {
                ImGui::SetCursorScreenPos(ImVec2(offset.x + m_Scripts[m_CurrentScript].Nodes[i].GetOutputSlotPos(j).x - node_slot_radius, offset.y + m_Scripts[m_CurrentScript].Nodes[i].GetOutputSlotPos(j).y - node_slot_radius));
                ImGui::InvisibleButton("output", ImVec2(node_slot_radius * 2, node_slot_radius * 2));
                if (ImGui::IsItemHovered())
                {
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        if (m_Scripts[m_CurrentScript].NodeInputSelected != -1)
                        {
                            if (i > m_Scripts[m_CurrentScript].SelectedConnectionNode)
                            {
                                auto from = m_Scripts[m_CurrentScript].Nodes.begin() + m_Scripts[m_CurrentScript].SelectedConnectionNode;
                                auto to = m_Scripts[m_CurrentScript].Nodes.begin() + i;
                                std::rotate(from, from + 1, to + 1);
                                m_Scripts[m_CurrentScript].Links.push_back(NodeLink(i - 1, j, i, m_Scripts[m_CurrentScript].NodeInputSelected));
                                for (int x = m_Scripts[m_CurrentScript].SelectedConnectionNode; x < static_cast<int>(m_Scripts[m_CurrentScript].Links.size()); x++)
                                {
                                    m_Scripts[m_CurrentScript].Links[x].InputIndex = m_Scripts[m_CurrentScript].Links[x].InputIndex - 1;
                                    m_Scripts[m_CurrentScript].Links[x].OutputIndex = m_Scripts[m_CurrentScript].Links[x].OutputIndex - 1;
                                }
                                m_Scripts[m_CurrentScript].NodeOutputSelected = -1;
                                m_Scripts[m_CurrentScript].NodeInputSelected = -1;
                                m_Scripts[m_CurrentScript].SelectedConnectionNode = -1;
                                m_Scripts[m_CurrentScript].Saved = false;
                                break;
                            }
                            m_Scripts[m_CurrentScript].Links.push_back(NodeLink(i, j, m_Scripts[m_CurrentScript].SelectedConnectionNode, m_Scripts[m_CurrentScript].NodeInputSelected));
                            m_Scripts[m_CurrentScript].NodeOutputSelected = -1;
                            m_Scripts[m_CurrentScript].NodeInputSelected = -1;
                            m_Scripts[m_CurrentScript].SelectedConnectionNode = -1;
                            m_Scripts[m_CurrentScript].Saved = false;
                        }
                        else
                        {
                            m_Scripts[m_CurrentScript].NodeOutputSelected = j;
                            m_Scripts[m_CurrentScript].SelectedConnectionNode = i;
                            m_Scripts[m_CurrentScript].Saved = false;
                        }
                    }
                }

                draw_list->AddCircleFilled(ImVec2(offset.x + m_Scripts[m_CurrentScript].Nodes[i].GetOutputSlotPos(j).x, offset.y + m_Scripts[m_CurrentScript].Nodes[i].GetOutputSlotPos(j).y), node_slot_radius, IM_COL32(150, 150, 150, 150));
            }
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
            {
                m_Scripts[m_CurrentScript].NodeInputSelected = -1;
                m_Scripts[m_CurrentScript].NodeOutputSelected = -1;
                m_Scripts[m_CurrentScript].SelectedConnectionNode = -1;
            }
            ImGui::PopID();
        }
        draw_list->ChannelsMerge();
    }

    // Open context menu
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !ImGui::IsAnyItemHovered())
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && m_Scripts.size() != 0)
        {
            m_Scripts[m_CurrentScript].SelectedNode = node_hovered_in_list = node_hovered_in_scene = -1;
            open_context_menu = true;
        }
    if (open_context_menu)
    {
        m_Scripts[m_CurrentScript].NodeInputSelected = -1;
        m_Scripts[m_CurrentScript].NodeOutputSelected = -1;
        m_Scripts[m_CurrentScript].SelectedConnectionNode = -1;

        ImGui::OpenPopup("context_menu");
        if (node_hovered_in_list != -1)
            m_Scripts[m_CurrentScript].SelectedNode = node_hovered_in_list;
        if (node_hovered_in_scene != -1)
            m_Scripts[m_CurrentScript].SelectedNode = node_hovered_in_scene;
    }

    // Draw context menu
    bool open_node_menu = false;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("Node Menu") && m_Scripts.size() != 0)
    {
        ImVec2 scene_pos;
        scene_pos.x = ImGui::GetMousePosOnOpeningCurrentPopup().x - offset.x;
        scene_pos.y = ImGui::GetMousePosOnOpeningCurrentPopup().y - offset.y;
        if (ImGui::MenuItem("Start")) { m_Scripts[m_CurrentScript].Nodes.push_back(ScriptingNode(scene_pos, "START", 0, 1, "Start")); }
        if (ImGui::MenuItem("Update")) { m_Scripts[m_CurrentScript].Nodes.push_back(ScriptingNode(scene_pos, "UPDATE", 0, 1, "Update")); }
        if (ImGui::MenuItem("Print")) { CreatePrintNode(m_Scripts[m_CurrentScript].Nodes, scene_pos, ""); }
        if (ImGui::MenuItem("Keyboard Input")) { CreateKeyboardInputNode(m_Scripts[m_CurrentScript].Nodes, scene_pos, 0, 0); }
        if (ImGui::MenuItem("Mouse Input")) { CreateMouseInputNode(m_Scripts[m_CurrentScript].Nodes, scene_pos, 0, 0); }
        if (ImGui::MenuItem("Apply Force"))
        {
            m_Scripts[m_CurrentScript].Nodes.push_back(ScriptingNode(scene_pos, "APPLYFORCE", 1, 1, "Apply Force"));
            m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].Nodes.size() - 1].NodeValues.push_back(ScriptingNodeValue(ScriptingNodeValue::Type::FLOAT, new float(0), "X"));
            m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].Nodes.size() - 1].NodeValues.push_back(ScriptingNodeValue(ScriptingNodeValue::Type::FLOAT, new float(0), "Y"));
        }
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("context_menu"))
    {
        ScriptingNode* node = m_Scripts[m_CurrentScript].SelectedNode != -1 ? &m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedNode] : NULL;
        if (node)
        {
            ImGui::Text(node->Name);
            ImGui::Separator();
            if (ImGui::MenuItem("Copy", NULL, false, false)) {}
            if (ImGui::MenuItem("Delete"))
            {
                for (int i = 0; i < static_cast<int>(m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedNode].NodeValues.size()); i++)
                {
                    switch (m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedNode].NodeValues[i].ValueType)
                    {
                    case ScriptingNodeValue::Type::FLOAT:
                        delete (float*)m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedNode].NodeValues[i].Value;
                        break;
                    case ScriptingNodeValue::Type::INT:
                        delete (int*)m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedNode].NodeValues[i].Value;
                        break;
                    case ScriptingNodeValue::Type::STRING:
                        delete (std::string*)m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedNode].NodeValues[i].Value;
                        break;
                    default:
                        break;
                    }
                    m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedNode].NodeValues[i].Value = nullptr;
                    m_Scripts[m_CurrentScript].Nodes[m_Scripts[m_CurrentScript].SelectedNode].NodeValues[i].ComboItems.clear();
                }
                m_Scripts[m_CurrentScript].Nodes.erase(m_Scripts[m_CurrentScript].Nodes.begin() + m_Scripts[m_CurrentScript].SelectedNode);
                for (int i = 0; i < static_cast<int>(m_Scripts[m_CurrentScript].Links.size()); i++)
                {
                    if (m_Scripts[m_CurrentScript].Links[i].InputIndex == m_Scripts[m_CurrentScript].SelectedNode || m_Scripts[m_CurrentScript].Links[i].OutputIndex == m_Scripts[m_CurrentScript].SelectedNode)
                    {
                        m_Scripts[m_CurrentScript].Links.erase(m_Scripts[m_CurrentScript].Links.begin() + i);
                    }
                }
            }
        }
        else
        {
            if (ImGui::MenuItem("Scripting Nodes")) { open_node_menu = true; }
        }
        ImGui::EndPopup();
    }
    if (open_node_menu)
    {
        ImGui::OpenPopup("Node Menu");
    }
    ImGui::PopStyleVar();

    // Scrolling
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f))
    {
        m_Scrolling.x = m_Scrolling.x + io.MouseDelta.x;
        m_Scrolling.y = m_Scrolling.y + io.MouseDelta.y;
    }

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::EndGroup();

    ImGui::End();
}