#include "Inspector.h"
#include <iostream>
#include <string>
#include "nfd.h"
#include "App.h"

Inspector::Inspector() {

}

std::string CreateFileDialog(const std::vector<std::string>& FileTypes, const std::vector<std::string>& FileExtensions)
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
        std::string node_name = Nodes[SelectedNode].Name;
        if (ImGui::InputText("Name", &node_name))
        {
            Nodes[SelectedNode].Name = node_name;
        }

        ImGui::PushID(0);
        ImGui::Text("Position");
        ImVec2 old_position = Nodes[SelectedNode].Position;
        if (ImGui::InputFloat("X", &Nodes[SelectedNode].Position.x) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesPosition(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_position);
        }
        if (ImGui::InputFloat("Y", &Nodes[SelectedNode].Position.y) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesPosition(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_position);
        }
        ImGui::PopID();

        ImGui::PushID(1);
        ImGui::Text("Size");
        ImVec2 old_size = Nodes[SelectedNode].Size;
        if (ImGui::InputFloat("X", &Nodes[SelectedNode].Size.x) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesSize(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_size);
        }
        if (ImGui::InputFloat("Y", &Nodes[SelectedNode].Size.y) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesSize(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_size);
        }
        ImGui::PopID();

        ImGui::Text("Angle");
        float old_angle = Nodes[SelectedNode].Angle;
        if (ImGui::InputFloat(" ", &Nodes[SelectedNode].Angle) && static_cast<int>(Nodes[SelectedNode].ChildIDs.size()) != 0)
        {
            ChangeChildNodesAngle(Nodes, SelectedNode - static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), static_cast<int>(Nodes[SelectedNode].ChildIDs.size()), SelectedNode, old_angle);
        }

        switch (Nodes[SelectedNode].NodeType)
        {
        case Node::Type::SPRITE:
            ImGui::Text("Texture");
            if (ImGui::Button((*(std::string*)Nodes[SelectedNode].NodeValues[0].Value).c_str()))
            {
                delete (std::string*)Nodes[SelectedNode].NodeValues[0].Value;
                Nodes[SelectedNode].NodeValues[0].Value = nullptr;
                Nodes[SelectedNode].NodeValues[0].Value = new std::string(CreateFileDialog({ "Image File" }, { "png,jpg,avif,jxl,tif,webp" }));
            }
            break;
        default:
            break;
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
        ImGui::Text(std::to_string(static_cast<int>(Nodes[SelectedNode].ChildIDs.size())).c_str());
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