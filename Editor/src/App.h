#ifndef APP_H
#define APP_H

#pragma once

#include <stdexcept>
#include <iostream>
#include <string>
#include "Viewport.h"
#include "imgui.h"
#include "Scripting.h"
#include "Inspector.h"
#include "SceneView.h"

class SDL_Window;
class SDL_Renderer;

struct Node
{
public:
    enum class Type
    {
        NODE = 0,
        SPRITE = 1,
        CAM = 2,
        PHYSICSBODY = 3,
        COLLIDER = 4
    };

    enum class ColliderType
    {
        BOX = 0,
        CIRCLE = 1,
        POLYGONS = 2
    };

    struct NodeValue
    {
    public:
        enum class Type
        {
            NULLTYPE = 0,
            STRING = 1,
            INT = 2,
            FLOAT = 3,
            VECTOR2 = 4,
            VECTORARRAY = 5
        };

        NodeValue(void* _Value, Type _Type)
        {
            Value = _Value;
            ValueType = _Type;
        }

        NodeValue(void* _Value, Type _Type, const std::vector<std::string>& _ComboValues)
        {
            Value = _Value;
            ValueType = _Type;

            ComboValues = _ComboValues;
        }

        NodeValue(std::vector<void*> _Value)
        {
            VectorValues = _Value;
            ValueType = Type::VECTORARRAY;
        }

        ~NodeValue()
        {
            switch (ValueType)
            {
            case Type::FLOAT:
                delete (float*)Value;
                break;
            case Type::INT:
                delete (int*)Value;
                break;
            case Type::STRING:
                delete (std::string*)Value;
                break;
            case Type::VECTOR2:
                delete (ImVec2*)Value;
                break;
            default:
                break;
            }

            Value = nullptr;
            ComboValues.clear();
        }
    public:
        void* Value = nullptr;

        std::vector<void*> VectorValues;

        Type ValueType = Type::NULLTYPE;

        std::vector<std::string> ComboValues;
    };
public:
    Node(ImVec2 _Position = ImVec2(0, 0), Type _NodeType = Type::NODE)
    {
        Position = _Position;
        NodeType = _NodeType;

        switch (NodeType)
        {
        case Node::Type::SPRITE:
            NodeValues.push_back(new Node::NodeValue(new std::string("None"), Node::NodeValue::Type::STRING));
            break;
        default:
            break;
        }

        ID = s_IDs;
        s_IDs += 1;
    }

    bool isEqual(const Node& Other) const
    {
        return (Other.ID == ID);
    }

    bool operator==(const Node& Other)
    {
        return isEqual(Other);
    }
    bool operator==(const Node* Other)
    {
        return isEqual(*Other);
    }
private:
    static int s_IDs;
public:
    int ID;
    
    Type NodeType = Type::NODE;

    std::string Name = "NodeName", Script = "None";

    ImVec2 Position = ImVec2(0, 0), Size = ImVec2(1, 1);

    float Angle = 0;

    std::vector<NodeValue*> NodeValues;

    bool IsChild = false;

    std::vector<int> ChildIDs;
    bool TreeOpen = false; // i didn't want to make a variable like this but i have no other solution
    
    int ParentID = -1;
};

class App
{
public:
    ~App();

    void Init();

    int Run();

    bool IsAppRunning() { return m_Running; }

    void LoadSceneFile(const std::string& FilePath);
private:
    void DockSpace();
private:
    bool m_Running = false;

    SDL_Window* m_Window = nullptr;

    SDL_Renderer* m_Renderer = nullptr;

    ImGuiIO* m_io = nullptr;

    Scripting m_Scripting;

    Viewport m_Viewport;

    Inspector m_Inspector;

    SceneView m_SceneView;

    std::vector<Node> m_Nodes;
};

#endif
