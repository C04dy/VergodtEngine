#include "Inspector.h"
#include <iostream>
#include <string>
#include "ImGui.h"
#include "imgui_stdlib.h"
#include "App.h"

Inspector::Inspector() {

}

void Inspector::InspectorSpace(std::vector<Node>& nodes, int& selectednode) {
    ImGui::Begin("Inspector");

    if (selectednode != -1) {
        std::string msg = nodes[selectednode].Name;
        if (ImGui::InputText("Name", &msg)) {
            nodes[selectednode].Name = msg;
        }

    }

    ImGui::End();
}
