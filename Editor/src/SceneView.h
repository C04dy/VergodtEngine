#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#pragma once

#include <vector>

struct Node;

class SceneView
{
public:
    SceneView() = default;

    void SceneViewSpace(std::vector<Node>& Nodes, int& SelectedNode, bool& Saved);
private:
    void CreateTreeNodes(std::vector<Node>& Nodes, int& SelectedNode, bool& Saved);
    int CreateChildTreeNodes(std::vector<Node>& Nodes, std::vector<Node*>& ChildNodes, int& SelectedNode, int CurrentParentIndex, bool& Saved);
};

#endif