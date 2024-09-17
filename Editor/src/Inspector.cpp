#include "Inspector.h"
#include <iostream>
#include <string>
#include "nfd.h"
#include "App.h"

static std::string CreateFileDialog(const std::vector<std::string>& FileTypes, const std::vector<std::string>& FileExtensions)
{
    nfdu8char_t* out_path;

    nfdu8filteritem_t filters[FileTypes.size()];

    for (int i = 0; i < static_cast<int>(FileTypes.size()); i++)
    {
        filters[i] = { FileTypes[i].c_str(), FileExtensions[i].c_str() };
    }

    nfdopendialogu8args_t args = { 0 };
    args.filterList = filters;
    args.filterCount = 2;
    nfdresult_t result = NFD_OpenDialogU8_With(&out_path, &args);
    if (result == NFD_OKAY)
    {
        NFD_FreePathU8(out_path);
        return out_path;
    }
    else if (result != NFD_CANCEL)
    {
        std::cout << "Error file did not opened: " << NFD_GetError() << '\n';
    }
    return "None";
}

void Inspector::InspectorSpace(std::vector<Node>& Nodes, int& SelectedNode, Scripting* ScriptingTab)
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
                delete (std::string*)Nodes[SelectedNode].NodeValues[0]->Value;
                Nodes[SelectedNode].NodeValues[0]->Value = nullptr;
                Nodes[SelectedNode].NodeValues[0]->Value = new std::string(CreateFileDialog({ "Image File" }, { "png,jpg,avif,jxl,tif,webp" }));
            }
            ImGui::PopID();
            break;
        case Node::Type::COLLIDER:
            ImGui::PushID(-2);
            if (Nodes[SelectedNode].NodeValues.size() != 0)
            {
                Node::ColliderType collider_type = *(Node::ColliderType*)Nodes[SelectedNode].NodeValues[0]->Value;
                if (ImGui::BeginCombo("Collider Type", Nodes[SelectedNode].NodeValues[0]->ComboValues[(int)collider_type].c_str()))
                {
                    for (int i = 0; i < static_cast<int>(Nodes[SelectedNode].NodeValues[0]->ComboValues.size()); i++)
                    {
                        const bool isSelected = (collider_type == (Node::ColliderType)i);
                        if (ImGui::Selectable(Nodes[SelectedNode].NodeValues[0]->ComboValues[i].c_str(), isSelected))
                        {
                            collider_type = (Node::ColliderType)i;
                            Node::ColliderType old_collider_type = *(Node::ColliderType*)Nodes[SelectedNode].NodeValues[0]->Value;

                            delete (Node::ColliderType*)Nodes[SelectedNode].NodeValues[0]->Value;
                            Nodes[SelectedNode].NodeValues[0]->Value = nullptr;
                            Nodes[SelectedNode].NodeValues[0]->Value = new Node::ColliderType(collider_type);

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
                                break;
                            case Node::ColliderType::CIRCLE:
                                Nodes[SelectedNode].NodeValues[1]->Value = new float(10.0f);
                                break;
                            case Node::ColliderType::POLYGONS:
                                Nodes[SelectedNode].NodeValues[1]->VectorValues.push_back(new ImVec2(0.0f, 0.0f));
                                break;
                            }
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

                    ImGui::InputFloat("X", &(*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value).x);
                    ImGui::SameLine();
                    ImGui::InputFloat("Y", &(*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->Value).y);

                    ImGui::PopItemWidth();
                    break;
                case (int)Node::ColliderType::CIRCLE:
                    ImGui::Text("Collider Radius");

                    ImGui::InputFloat(" ", &(*(float*)Nodes[SelectedNode].NodeValues[1]->Value));
                    break;
                case (int)Node::ColliderType::POLYGONS:
                    for (int i = 0; i < static_cast<int>(Nodes[SelectedNode].NodeValues[1]->VectorValues.size()); i++)
                    {
                        ImGui::PushID(i);
                        ImGui::PushItemWidth((ImGui::GetWindowSize().x / 2) - 25);
                        ImGui::InputFloat("X", &(*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->VectorValues[i]).x);
                        ImGui::SameLine();
                        ImGui::InputFloat("Y", &(*(ImVec2*)Nodes[SelectedNode].NodeValues[1]->VectorValues[i]).y);
                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }

                    if (ImGui::Button("Add"))
                    {
                        Nodes[SelectedNode].NodeValues[1]->VectorValues.push_back(new ImVec2(0.0f, 0.0f));
                    }
                    break;
                }
            }
            ImGui::PopID();
            break;
        default:
            break;
        }

        ImGui::PushID(-1);
        std::string node_name = Nodes[SelectedNode].Name;
        ImGui::Text("Name");
        if (ImGui::InputText(" ", &node_name))
        {
            Nodes[SelectedNode].Name = node_name;
        }
        ImGui::PopID();

        ImGui::PushID(0);
        ImGui::Text("Position");
        ImVec2 old_position = Nodes[SelectedNode].Position;
        ImGui::PushItemWidth((ImGui::GetWindowSize().x / 2) - 25);
        if (ImGui::InputFloat("X", &Nodes[SelectedNode].Position.x) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesPosition(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_position);
        }
        ImGui::SameLine();
        if (ImGui::InputFloat("Y", &Nodes[SelectedNode].Position.y) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesPosition(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_position);
        }
        ImGui::PopItemWidth();
        ImGui::PopID();

        ImGui::PushID(1);
        ImGui::Text("Size");
        ImVec2 old_size = Nodes[SelectedNode].Size;
        ImGui::PushItemWidth((ImGui::GetWindowSize().x / 2) - 25);
        if (ImGui::InputFloat("X", &Nodes[SelectedNode].Size.x) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesSize(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_size);
        }
        ImGui::SameLine();
        if (ImGui::InputFloat("Y", &Nodes[SelectedNode].Size.y) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesSize(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_size);
        }
        ImGui::PopItemWidth();
        ImGui::PopID();

        ImGui::Text("Angle");
        float old_angle = Nodes[SelectedNode].Angle;
        if (ImGui::InputFloat(" ", &Nodes[SelectedNode].Angle) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesAngle(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_angle);
        }

        ImGui::Text("Script");
        if (ImGui::Button(Nodes[SelectedNode].Script.c_str()))
        {
            if (Nodes[SelectedNode].Script != "None")
            {
                ScriptingTab->LoadScript(Nodes[SelectedNode].Script);
            }
            else
            {
                Nodes[SelectedNode].Script = CreateFileDialog({"VergodtScript File"}, {"verscript"});
            }
        }

        ImGui::Text(std::to_string(SelectedNode).c_str());
    }

    ImGui::End();
}

void Inspector::ChangeChildNodesPosition(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, const ImVec2& old_positionition)
{
    bool IsX = Nodes[CurrentNode].Position.x != old_positionition.x;
    for (int i = StartPose; i < StartPose + Lentgth; i++)
    {
        Nodes[i].Position.x += (Nodes[CurrentNode].Position.x - old_positionition.x) * int(IsX);
        Nodes[i].Position.y += (Nodes[CurrentNode].Position.y - old_positionition.y) * int(!IsX);

        if (static_cast<int>(Nodes[i].ChildIDs.size()) != 0)
        {
            ChangeChildNodesPosition(Nodes, i - static_cast<int>(Nodes[i].ChildIDs.size()), static_cast<int>(Nodes[i].ChildIDs.size()), i, old_positionition);
        }
    }
}

void Inspector::ChangeChildNodesSize(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, const ImVec2& OldSize)
{
    bool IsX = Nodes[CurrentNode].Size.x != OldSize.x;
    for (int i = StartPose; i < StartPose + Lentgth; i++)
    {
        Nodes[i].Size.x += (Nodes[CurrentNode].Size.x - OldSize.x) * int(IsX);
        Nodes[i].Size.y += (Nodes[CurrentNode].Size.y - OldSize.y) * int(!IsX);

        if (static_cast<int>(Nodes[i].ChildIDs.size()) != 0)
        {
            ChangeChildNodesSize(Nodes, i - static_cast<int>(Nodes[i].ChildIDs.size()), static_cast<int>(Nodes[i].ChildIDs.size()), i, OldSize);
        }
    }
}

void Inspector::ChangeChildNodesAngle(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, float OldAngle)
{
    for (int i = StartPose; i < StartPose + Lentgth; i++)
    {
        Nodes[i].Angle += (Nodes[CurrentNode].Angle - OldAngle);

        if (static_cast<int>(Nodes[i].ChildIDs.size()) != 0)
        {
            ChangeChildNodesAngle(Nodes, i - static_cast<int>(Nodes[i].ChildIDs.size()), static_cast<int>(Nodes[i].ChildIDs.size()), i, OldAngle);
        }
    }
}