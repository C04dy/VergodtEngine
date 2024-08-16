#ifndef SCRIPTING_H
#define SCRIPTING_H

#pragma once

#include "imgui.h"
#include <string>
#include <vector>

class Scripting
{
public:
    Scripting();

    void ScriptingSpace();

    void ChangeScript(const std::string& ScriptPath);

    void SaveScript(const std::string& ScriptPath);
private:
    enum class ScriptingNodeValueType {
        NULLTYPE = 0,
        FLOAT = 1,
        INT = 2,
        STRING = 3
    };

    struct ScriptingNodeValue {
    private:
        void* Value = nullptr;
        std::string StringValue;
    public:
        ScriptingNodeValueType Type = ScriptingNodeValueType::NULLTYPE;
        std::string ValueName = "Val";
        
        std::vector<std::string> ComboItems;

        void* GetValue() {
            if (Type == ScriptingNodeValueType::STRING) {
                return (void*)StringValue.data();
            }
            return Value;
        }

        void SetValue(const std::string& val) {
            StringValue = val;
        }

        void SetValue(void* val) {
            Value = val;
        }

        ScriptingNodeValue(ScriptingNodeValueType type, void* val, int id, const std::string& name) {
            Value = val;
            Type = type;
            ValueName = name;
        }
        ScriptingNodeValue(ScriptingNodeValueType type, const std::string& val, int id, const std::string& name) {
            StringValue = val;
            Type = type;
            ValueName = name;
        }
    };

    struct ScriptingNode {
        int ID;
        char Name[32] = "ScriptingNode";
        ImVec2 Pos, Size;
        int InputsCount = 1, OutputsCount = 1;
        std::vector<ScriptingNodeValue> NodeValues;
        char Type[32] = "NULL";

        ScriptingNode(int id, const ImVec2& pos, const char* type, int i = 1, int o = 1, const char* nodename = "ScriptingNode") {
            ID = id;
            Pos = pos;
            strcpy(Name, nodename);
            InputsCount = i;
            OutputsCount = o;
            strcpy(Type, type);
        }

        ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)InputsCount + 1)); }
        ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)OutputsCount + 1)); }
    };
    
    struct NodeLink {
        int InputIdx, InputSlot, OutputIdx, OutputSlot;

        NodeLink(int input_idx, int input_slot, int output_idx, int output_slot) { InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot; }
    };

    ImVector<ScriptingNode> nodes;
    ImVector<NodeLink> links;
    ImVec2 scrolling = ImVec2(0.0f, 0.0f);
    bool show_grid = true;
    int node_selected = -1;

    int SelectedConnectionNode = -1;
    int NodeInputSelected = -1;
    int NodeOutputSelected = -1;

    std::string CurrentScriptPath;
};

#endif