#ifndef INSPECTOR_H
#define INSPECTOR_H

#pragma once

#include <vector>
#include "imgui.h"
#include "imgui_stdlib.h"

struct Node;
class Scripting;
class Project;

class Inspector
{
public:
    Inspector() = default;

    void InspectorSpace(std::vector<Node>& Nodes, int& SelectedNode, Scripting& ScriptingTab, bool& Saved, Project& CurrentProject);
private:
    void ChangeChildNodesPosition(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, const ImVec2& OldPosition);

    void ChangeChildNodesSize(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, const ImVec2& OldSize);

    void ChangeChildNodesAngle(std::vector<Node>& Nodes, int StartPose, int Lentgth, int CurrentNode, float OldAngle);
};

#endif
