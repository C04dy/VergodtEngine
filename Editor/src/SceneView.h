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

    void SceneViewSpace(std::vector<Node>& Nodes, int& SelectedNode);
private:
    void CreateTreeNodes(std::vector<Node>& Nodes, int& SelectedNode);
    int CreateChildTreeNodes(std::vector<Node>& Nodes, std::vector<Node*>& ChildNodes, int& SelectedNode, int CurrentParentIndex);
};

#endif