#include "Inspector.h"
#include <iostream>
#include <string>
#include "App.h"
#include "StringFunctions.h"

void Inspector::InspectorSpace(std::vector<Node>& Nodes, int& SelectedNode, Scripting& ScriptingTab, bool& Saved, Project& CurrentProject)
{
    ImGui::Begin("Inspector");

    if (SelectedNode != -1)
    {
        switch (Nodes[SelectedNode].NodeType)
        {
        case Node::Type::SPRITE:
            ImGui::PushID(-2);
            ImGui::Text("Texture");
            if (ImGui::Button((*(std::string*)Nodes[SelectedNode].NodeValues[0]->Value).c_str()))
            {
                std::string new_path = CreateFileOpenDialog({ "Image File" }, { "png,jpg,avif,jxl,tif,webp" });
                if (new_path != "None")
                {
                    *(std::string*)Nodes[SelectedNode].NodeValues[0]->Value = RemoveFromLine(new_path, CurrentProject.GetProjectLocation());
                    CurrentProject.LoadTextureFromFile(new_path, Nodes[SelectedNode]);
                }
                Saved = false;
            }
            ImGui::PopID();
            break;
        case Node::Type::COLLIDER: {
            ImGui::PushID(-2);
            Node::ColliderType collider_type = *(Node::ColliderType*)Nodes[SelectedNode].NodeValues[0]->Value;
            if (ImGui::BeginCombo("Collider Type", Nodes[SelectedNode].NodeValues[0]->ComboValues[(int)collider_type].c_str()))
            {
                for (int i = 0; i < static_cast<int>(Nodes[SelectedNode].NodeValues[0]->ComboValues.size()); i++)
                {
                    const bool isSelected = (collider_type == (Node::ColliderType)i);
                    if (ImGui::Selectable(Nodes[SelectedNode].NodeValues[0]->ComboValues[i].c_str(), isSelected))
                    {
                        Node::ColliderType old_collider_type = *(Node::ColliderType*)Nodes[SelectedNode].NodeValues[0]->Value;
                        collider_type = (Node::ColliderType)i;
                        *(Node::ColliderType*)Nodes[SelectedNode].NodeValues[0]->Value = collider_type;

                        switch (old_collider_type)
                        {
                        case Node::ColliderType::BOX:
                            delete (ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value;
                            Nodes[SelectedNode].NodeValues[1]->Value = nullptr;
                            break;
                        case Node::ColliderType::CIRCLE:
                            delete (float*)Nodes[SelectedNode].NodeValues[1]->Value;
                            Nodes[SelectedNode].NodeValues[1]->Value = nullptr;
                            break;
                        case Node::ColliderType::POLYGONS:
                            Nodes[SelectedNode].NodeValues[1]->VectorValues.clear();
                            break;
                        }

                        switch (collider_type)
                        {
                        case Node::ColliderType::BOX:
                            Nodes[SelectedNode].NodeValues[1]->Value = new ImVec2(10.0f, 10.0f);
                            Nodes[SelectedNode].NodeValues[1]->ValueType = Node::NodeValue::Type::VECTOR2;
                            break;
                        case Node::ColliderType::CIRCLE:
                            Nodes[SelectedNode].NodeValues[1]->Value = new float(10.0f);
                            Nodes[SelectedNode].NodeValues[1]->ValueType = Node::NodeValue::Type::FLOAT;
                            break;
                        case Node::ColliderType::POLYGONS:
                            Nodes[SelectedNode].NodeValues[1]->VectorValues.push_back(new ImVec2(10.0f, 10.0f));
                            Nodes[SelectedNode].NodeValues[1]->ValueType = Node::NodeValue::Type::VECTORARRAY;
                            Nodes[SelectedNode].NodeValues[1]->VectorValueType = Node::NodeValue::Type::VECTOR2;
                            break;
                        }
                        Saved = false;
                    }
                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            switch (*(int*)Nodes[SelectedNode].NodeValues[0]->Value)
            {
            case (int)Node::ColliderType::BOX:
                ImGui::Text("Collider Size");

                ImGui::PushItemWidth((ImGui::GetWindowSize().x / 2) - 25);

                if (ImGui::InputFloat("X", &(*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value).x))
                    Saved = false;
                ImGui::SameLine();
                if (ImGui::InputFloat("Y", &(*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value).y))
                    Saved = false;

                if (((ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value)->x < 0.0f)
                    (*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value).x = std::abs((*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value).x);
                if (((ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value)->y < 0.0f)
                    (*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value).y = std::abs((*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value).y);

                ImGui::PopItemWidth();
                break;
            case (int)Node::ColliderType::CIRCLE:
                ImGui::Text("Collider Radius");

                if (ImGui::InputFloat(" ", ((float*)Nodes[SelectedNode].NodeValues[1]->Value)))
                    Saved = false;

                if ((*(float*)Nodes[SelectedNode].NodeValues[1]->Value) < 0.0f)
                    (*(float*)Nodes[SelectedNode].NodeValues[1]->Value) = std::abs((*(float*)Nodes[SelectedNode].NodeValues[1]->Value));
                break;
            case (int)Node::ColliderType::POLYGONS:
                for (int i = 0; i < static_cast<int>(Nodes[SelectedNode].NodeValues[1]->VectorValues.size()); i++)
                {
                    ImGui::PushID(i);
                    ImGui::PushItemWidth((ImGui::GetWindowSize().x / 2) - 50);
                    if (ImGui::InputFloat("X", &(*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->VectorValues[i]).x, 0, 0, "%.3f"))
                        Saved = false;

                    ImGui::SameLine();
                    if (ImGui::InputFloat("Y", &(*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->VectorValues[i]).y))
                        Saved = false;

                    ImGui::SameLine();
                    if (ImGui::Button("Delete"))
                    {
                        Nodes[SelectedNode].NodeValues[1]->VectorValues.erase(Nodes[SelectedNode].NodeValues[1]->VectorValues.begin() + i);
                        Saved = false;
                    }
                    ImGui::PopItemWidth();

                    ImGui::PopID();
                }

                if (ImGui::Button("Add"))
                {
                    Nodes[SelectedNode].NodeValues[1]->VectorValues.push_back(new ImVec2(0.0f, 0.0f));
                    Saved = false;
                }
                break;
            }
            ImGui::PopID();
        }    break;
        case Node::Type::PHYSICSBODY: {
            ImGui::PushID(-2);

            Node::PhysicsbodyType physicsbody_type = *(Node::PhysicsbodyType*)Nodes[SelectedNode].NodeValues[0]->Value;
            if (ImGui::BeginCombo("Physicsbody Type", Nodes[SelectedNode].NodeValues[0]->ComboValues[(int)physicsbody_type].c_str()))
            {
                for (int i = 0; i < static_cast<int>(Nodes[SelectedNode].NodeValues[0]->ComboValues.size()); i++)
                {
                    const bool isSelected = (physicsbody_type == (Node::PhysicsbodyType)i);
                    if (ImGui::Selectable(Nodes[SelectedNode].NodeValues[0]->ComboValues[i].c_str(), isSelected))
                    {
                        *(Node::PhysicsbodyType*)Nodes[SelectedNode].NodeValues[0]->Value = (Node::PhysicsbodyType)i;
                        Saved = false;
                    }
                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            if (ImGui::InputFloat("Friction", (float*)Nodes[SelectedNode].NodeValues[1]->Value))
                Saved = false;
            if (ImGui::InputFloat("Density", (float*)Nodes[SelectedNode].NodeValues[2]->Value))
                Saved = false;

            ImGui::PopID();
        }   break;
        default:
            break;
        }

        ImGui::PushID(-1);
        std::string node_name = Nodes[SelectedNode].Name;
        ImGui::Text("Name");
        if (ImGui::InputText(" ", &node_name))
        {
            Nodes[SelectedNode].Name = node_name;
            Saved = false;
        }
        ImGui::PopID();

        ImGui::PushID(0);
        ImGui::Text("Position");
        ImVec2 old_position = Nodes[SelectedNode].Position;
        ImGui::PushItemWidth((ImGui::GetWindowSize().x / 2) - 25);
        if (ImGui::InputFloat("X", &Nodes[SelectedNode].Position.x))
        {
            if (static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
                ChangeChildNodesPosition(Nodes, SelectedNode, old_position);
            Saved = false;
        }
        ImGui::SameLine();
        if (ImGui::InputFloat("Y", &Nodes[SelectedNode].Position.y))
        {
            if (static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
                ChangeChildNodesPosition(Nodes, SelectedNode, old_position);
            Saved = false;
        }
        ImGui::PopItemWidth();
        ImGui::PopID();

        ImGui::PushID(1);
        ImGui::Text("Size");
        ImVec2 old_size = Nodes[SelectedNode].Size;
        ImGui::PushItemWidth((ImGui::GetWindowSize().x / 2) - 25);
        if (ImGui::InputFloat("X", &Nodes[SelectedNode].Size.x))
        {
            if (static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
                ChangeChildNodesSize(Nodes, SelectedNode, old_size);
            Saved = false;
        }
        ImGui::SameLine();
        if (ImGui::InputFloat("Y", &Nodes[SelectedNode].Size.y))
        {
            if (static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
                ChangeChildNodesSize(Nodes, SelectedNode, old_size);
            Saved = false;
        }
        ImGui::PopItemWidth();
        ImGui::PopID();

        ImGui::Text("Angle");
        float old_angle = Nodes[SelectedNode].Angle;
        if (ImGui::InputFloat(" ", &Nodes[SelectedNode].Angle))
        {
            if (static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
                ChangeChildNodesAngle(Nodes, SelectedNode, old_angle);
            Saved = false;
        }

        ImGui::Text("Script");
        if (ImGui::Button(Nodes[SelectedNode].Script.c_str()))
        {
            if (Nodes[SelectedNode].Script != "None")
            {
                ScriptingTab.LoadScript(CurrentProject.GetProjectLocation() + Nodes[SelectedNode].Script);
            }
            else
            {
                Nodes[SelectedNode].Script = CreateFileSaveDialog({"VergodtScript File"}, {"verscript"});
                Nodes[SelectedNode].Script = RemoveFromLine(Nodes[SelectedNode].Script, CurrentProject.GetProjectLocation());
            }
            Saved = false;
        }
        if (Nodes[SelectedNode].Script != "None")
        {
            ImGui::SameLine();
            if (ImGui::Button("Remove"))
            {
                Nodes[SelectedNode].Script = "None";
                Saved = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Change"))
            {
                Nodes[SelectedNode].Script = CreateFileOpenDialog({ "VergodtScript File" }, { "verscript" });
                Nodes[SelectedNode].Script = RemoveFromLine(Nodes[SelectedNode].Script, CurrentProject.GetProjectLocation());
                Saved = false;
            }
        }
    }

    ImGui::End();
}

void Inspector::ChangeChildNodesPosition(std::vector<Node>& Nodes, int CurrentNode, const ImVec2& OldPosition)
{
    bool IsX = Nodes[CurrentNode].Position.x != OldPosition.x;
    for (int i = 0; i < static_cast<int>(Nodes.size()); i++)
    {
        for (int j = 0; j < static_cast<int>(Nodes[CurrentNode].ChildIDs.size()); j++)
        {
            if (Nodes[i].ID == Nodes[CurrentNode].ChildIDs[j])
            {
                Nodes[i].Position.x += (Nodes[CurrentNode].Position.x - OldPosition.x) * int(IsX);
                Nodes[i].Position.y += (Nodes[CurrentNode].Position.y - OldPosition.y) * int(!IsX);

                if (static_cast<int>(Nodes[i].ChildIDs.size()) != 0)
                {
                    ChangeChildNodesPosition(Nodes, i, OldPosition);
                }
            }
        }
    }
}

void Inspector::ChangeChildNodesSize(std::vector<Node>& Nodes, int CurrentNode, const ImVec2& OldSize)
{
    bool IsX = Nodes[CurrentNode].Size.x != OldSize.x;
    for (int i = 0; i < static_cast<int>(Nodes.size()); i++)
    {
        for (int j = 0; j < static_cast<int>(Nodes[CurrentNode].ChildIDs.size()); j++)
        {
            if (Nodes[i].ID == Nodes[CurrentNode].ChildIDs[j])
            {
                Nodes[i].Size.x += (Nodes[CurrentNode].Size.x - OldSize.x) * int(IsX);
                Nodes[i].Size.y += (Nodes[CurrentNode].Size.y - OldSize.y) * int(!IsX);

                if (static_cast<int>(Nodes[i].ChildIDs.size()) != 0)
                {
                    ChangeChildNodesSize(Nodes, i, OldSize);
                }
            }
        }
    }
}

void Inspector::ChangeChildNodesAngle(std::vector<Node>& Nodes, int CurrentNode, float OldAngle)
{
    for (int i = 0; i < static_cast<int>(Nodes.size()); i++)
    {
        for (int j = 0; j < static_cast<int>(Nodes[CurrentNode].ChildIDs.size()); j++)
        {
            if (Nodes[i].ID == Nodes[CurrentNode].ChildIDs[j])
            {
                Nodes[i].Angle += (Nodes[CurrentNode].Angle - OldAngle);

                if (static_cast<int>(Nodes[i].ChildIDs.size()) != 0)
                {
                    ChangeChildNodesAngle(Nodes, i, OldAngle);
                }
            }
        }
    }
}