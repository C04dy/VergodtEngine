#pragma once

#include "Node.h"

class Camera : public Node
{
public:
    ~Camera() {}
    
    bool IsActive = false;
};