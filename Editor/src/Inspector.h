#ifndef INSPECTOR_H
#define INSPECTOR_H

#pragma once

#include <vector>
#include "imgui.h"
#include "imgui_stdlib.h"

struct Node;
class Scripting;

class Inspector {
public:
    Inspector();

    void InspectorSpace(std::vector<Node>& nodes, int& selectednode, Scripting* ScriptingTab);
private:

    void ChangeChildNodesPosition(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, const ImVec2& OldPos);

    void ChangeChildNodesSize(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, const ImVec2& OldSize);

    void ChangeChildNodesAngle(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, float OldAngle);

};

#endif
