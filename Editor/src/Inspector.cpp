#include "Inspector.h"
#include <iostream>
#include <string>
#include "nfd.h"
#include "App.h"

Inspector::Inspector() {

}

std::string CreateFileDialog(const std::vector<std::string>& FileTypes, const std::vector<std::string>& FileExtensions) {
    nfdu8char_t* outPath;

    nfdu8filteritem_t filters[FileTypes.size()];

    for (int i = 0; i < FileTypes.size(); i++) {
        filters[i] = { FileTypes[i].c_str(), FileExtensions[i].c_str() };
    }

    nfdopendialogu8args_t args = { 0 };
    args.filterList = filters;
    args.filterCount = 2;
    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
    if (result == NFD_OKAY) {
        NFD_FreePathU8(outPath);
        return outPath;
    } else if (result != NFD_CANCEL) {
        std::cout << "Error file did not opened: " << NFD_GetError() << '\n';
    }
    return "None";
}

void Inspector::InspectorSpace(std::vector<Node>& nodes, int& selectednode, Scripting* ScriptingTab) {
    ImGui::Begin("Inspector");

    if (selectednode != -1) {
        std::string msg = nodes[selectednode].Name;
        if (ImGui::InputText("Name", &msg)) {
            nodes[selectednode].Name = msg;
        }

        ImGui::PushID(0);
        ImGui::Text("Position");
        ImVec2 OldPos = nodes[selectednode].Position;
        if (ImGui::InputFloat("X", &nodes[selectednode].Position.x) && nodes[selectednode].ChildCount != 0) {
            ChangeChildNodesPosition(nodes, selectednode - nodes[selectednode].ChildCount, nodes[selectednode].ChildCount, selectednode, OldPos);
        }
        if (ImGui::InputFloat("Y", &nodes[selectednode].Position.y) && nodes[selectednode].ChildCount != 0) {
            ChangeChildNodesPosition(nodes, selectednode - nodes[selectednode].ChildCount, nodes[selectednode].ChildCount, selectednode, OldPos);
        }
        ImGui::PopID();

        ImGui::PushID(1);
        ImGui::Text("Size");
        ImVec2 OldSize = nodes[selectednode].Size;
        if (ImGui::InputFloat("X", &nodes[selectednode].Size.x) && nodes[selectednode].ChildCount != 0) {
            ChangeChildNodesSize(nodes, selectednode - nodes[selectednode].ChildCount, nodes[selectednode].ChildCount, selectednode, OldSize);
        }
        if (ImGui::InputFloat("Y", &nodes[selectednode].Size.y) && nodes[selectednode].ChildCount != 0) {
            ChangeChildNodesSize(nodes, selectednode - nodes[selectednode].ChildCount, nodes[selectednode].ChildCount, selectednode, OldSize);
        }
        ImGui::PopID();

        ImGui::Text("Angle");
        float OldAngle = nodes[selectednode].Angle;
        if (ImGui::InputFloat("Angle", &nodes[selectednode].Angle) && nodes[selectednode].ChildCount != 0) {
            ChangeChildNodesAngle(nodes, selectednode - nodes[selectednode].ChildCount, nodes[selectednode].ChildCount, selectednode, OldAngle);
        }

        ImGui::Text("Script");
        if (ImGui::Button(nodes[selectednode].Script.c_str())) {
            if (nodes[selectednode].Script != "None") {
                ScriptingTab->LoadScript(nodes[selectednode].Script);
            } else {
                nodes[selectednode].Script = CreateFileDialog({"VergodtScript File"}, {"verscript"});
            }
        }
    }

    ImGui::End();
}

void Inspector::ChangeChildNodesPosition(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, const ImVec2& OldPos) {
    bool IsX = Nodes[CurrentNode].Position.x != OldPos.x;
    for (int i = StartPose; i < StartPose + Lentgth; i++) {
        Nodes[i].Position.x += (Nodes[CurrentNode].Position.x - OldPos.x) * int(IsX);
        Nodes[i].Position.y += (Nodes[CurrentNode].Position.y - OldPos.y) * int(!IsX);

        if (Nodes[i].ChildCount != 0) {
            ChangeChildNodesPosition(Nodes, i - Nodes[i].ChildCount, Nodes[i].ChildCount, i, OldPos);
        }
    }
}

void Inspector::ChangeChildNodesSize(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, const ImVec2& OldSize) {
    bool IsX = Nodes[CurrentNode].Size.x != OldSize.x;
    for (int i = StartPose; i < StartPose + Lentgth; i++) {
        Nodes[i].Size.x += (Nodes[CurrentNode].Size.x - OldSize.x) * int(IsX);
        Nodes[i].Size.y += (Nodes[CurrentNode].Size.y - OldSize.y) * int(!IsX);

        if (Nodes[i].ChildCount != 0) {
            ChangeChildNodesSize(Nodes, i - Nodes[i].ChildCount, Nodes[i].ChildCount, i, OldSize);
        }
    }
}

void Inspector::ChangeChildNodesAngle(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, float OldAngle) {
    for (int i = StartPose; i < StartPose + Lentgth; i++) {
        Nodes[i].Angle += (Nodes[CurrentNode].Angle - OldAngle);

        if (Nodes[i].ChildCount != 0) {
            ChangeChildNodesAngle(Nodes, i - Nodes[i].ChildCount, Nodes[i].ChildCount, i, OldAngle);
        }
    }
}