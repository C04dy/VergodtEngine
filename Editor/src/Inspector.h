#ifndef INSPECTOR_H
#define INSPECTOR_H

#pragma once

#include <vector>

struct Node;

class Inspector {
public:
    Inspector();

    void InspectorSpace(std::vector<Node>& nodes, int& selectednode);
private:
};

#endif
