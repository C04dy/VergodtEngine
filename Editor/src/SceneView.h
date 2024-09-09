#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#pragma once

#include <vector>

struct Node;

class SceneView
{
public:
    SceneView();
    ~SceneView();

    void SceneViewSpace(const std::vector<Node>& Nodes, int& SelectedNode);
private:
    void CreateTreeNodes(const std::vector<Node>& Nodes, int& SelectedNode, int StartPoint = 0, int Length = -1, bool IsNodesChild = false);
};

#endif