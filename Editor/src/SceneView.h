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

    void SceneViewSpace(const std::vector<Node>& nodes, int& selectednode);
private:
    void CreateTreeNodes(const std::vector<Node>& nodes, int& selectednode, int startpoint = 0, int length = -1, bool isnodeschild = false);
};

#endif