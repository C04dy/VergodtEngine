#ifndef SCRIPTING_H
#define SCRIPTING_H

#pragma once

#include "imgui.h"
#include <string>

class Scripting
{
public:
    Scripting();

    void ScriptingSpace();

private:
    struct ScriptingNode {
        int ID;
        char Name[32] = "ScriptingNode";
        ImVec2 Pos, Size;
        int InputsCount = 1, OutputsCount = 1;

        ScriptingNode(int id, const ImVec2& pos) { 
            ID = id;
            Pos = pos;
        }

        ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)InputsCount + 1)); }
        ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)OutputsCount + 1)); }
    };

    struct Update : ScriptingNode {
        Update(int id, const ImVec2& pos) : ScriptingNode(id, pos) {
            strcpy(Name, "Update");
            InputsCount = 0;
            OutputsCount = 1;
        }
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
};

#endif