#pragma once
#include "Node.h"
class Camera : public Node
{
public:
    Camera() : Node::Node("") {};
    bool IsActive = false;
};