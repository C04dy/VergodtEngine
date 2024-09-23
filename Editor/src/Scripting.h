#ifndef SCRIPTING_H
#define SCRIPTING_H

#pragma once

#include "imgui.h"
#include <string>
#include <vector>
#include <iostream>

class Scripting
{
public:
    struct ScriptingNodeValue
    {
        enum class Type
        {
            NULLTYPE = 0,
            FLOAT = 1,
            INT = 2,
            STRING = 3
        };
        
        ScriptingNodeValue(Type _Type, void* _Value, const std::string& Name)
        {
            Value = _Value;
            ValueType = _Type;
            ValueName = Name;
        }

        void* Value = nullptr;

        Type ValueType = Type::NULLTYPE;

        std::string ValueName = "Val";

        std::vector<std::string> ComboValues;
    };

    struct ScriptingNode
    {
        ScriptingNode(const ImVec2& _Position, const char* _Type, int Inputs = 1, int Outputs = 1, const char* NodeName = "ScriptingNode")
        {
            Position = _Position;
            strcpy(Name, NodeName);
            InputsCount = Inputs;
            OutputsCount = Outputs;
            strcpy(Type, _Type);
        }

        ImVec2 GetInputSlotPos(int SlotNo) const { return ImVec2(Position.x, Position.y + Size.y * ((float)SlotNo+ 1) / ((float)InputsCount + 1)); }
        ImVec2 GetOutputSlotPos(int SlotNo) const { return ImVec2(Position.x + Size.x, Position.y + Size.y * ((float)SlotNo + 1) / ((float)OutputsCount + 1)); }

        char Name[32] = "ScriptingNode";
        ImVec2 Position, Size;
        int InputsCount = 1, OutputsCount = 1;
        std::vector<ScriptingNodeValue> NodeValues;
        char Type[32] = "NULL";
    };
    
    struct NodeLink
    {
        NodeLink(int _InputIndex, int _InputSlot, int _OutputIndex, int _OutputSlot) { InputIndex = _InputIndex; InputSlot = _InputSlot; OutputIndex = _OutputIndex; OutputSlot = _OutputSlot; }

        int InputIndex, InputSlot, OutputIndex, OutputSlot;
    };

    class Script
    {
    public:
        std::vector<ScriptingNode> Nodes;
        std::vector<NodeLink> Links;
        int SelectedNode = -1;

        int SelectedConnectionNode = -1;
        int NodeInputSelected = -1;
        int NodeOutputSelected = -1;

        bool Saved = true;

        std::string CurrentScriptPath;

        std::string ScriptName;
    };
public:
    Scripting() = default;

    void ScriptingSpace();

    void LoadScript(const std::string& ScriptPath);

    void SaveScript(const std::string& ScriptPath);
private:
    std::vector<Script> m_Scripts;

    bool m_ShowGrid = true;
    ImVec2 m_Scrolling = ImVec2(0.0f, 0.0f);

    int m_CurrentScript = 0;

    float m_Zoom = 1;
};

#endif