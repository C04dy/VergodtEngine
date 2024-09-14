#include <string>
#include <iostream>
#include "StringFunctions.h"

void ScriptError(const std::string& Message, const std::string& ScriptPath)
{
    std::cout << "Script File Error\n" << Message << '\n' << "Script File Path = " << ScriptPath << '\n';
}

void SetScript(Node* _Node, const std::string& Line, InputManager* Input)
{
    if (IsLineExist(Line, "[SCRIPT="))
    {
        std::string line;
        std::string file_path = GetLineBetween(Line, "[SCRIPT=", "]");
        std::ifstream script_file(file_path);

        if (script_file.fail())
        {
            ScriptError("Script File did not found.", file_path);
            return;
        }

        StartNode* start_node = new StartNode;
        UpdateNode* update_node = new UpdateNode;
        _Node->Script = new VisualScript;

        _Node->Script->InitVisualScript(start_node, update_node);

        std::vector<ScriptingNode*> all_script_nodes;

        while (std::getline(script_file, line))
        {
            std::string node_type = GetLineBetween(line, "[TYPE=", "]");

            if (node_type == "START")
            {
                all_script_nodes.push_back(start_node);
            }
            else if (node_type == "UPDATE")
            {
                all_script_nodes.push_back(update_node);
            }
            else if (node_type == "PRINT")
            {
                PrintNode* print_node = new PrintNode;

                print_node->Message = GetLineBetween(line, "(", ")");

                all_script_nodes.push_back(print_node);

                if (GetLineBetween(line, "[CONNECTEDID=", "]") != "NULL")
                    all_script_nodes[std::stoi(GetLineBetween(line, "[CONNECTEDID=", "]"))]->ConnectedNodes.push_back(print_node); 
            }
            else if (node_type == "KEYBOARDINPUT")
            {
                KeyboardInputNode* keyboard_node = new KeyboardInputNode(Input, 
                                                            GetLineBetween(line, "[KEY=", "]"),
                                                            GetLineBetween(line, "[INPUTTYPE=", "]"));
                all_script_nodes.push_back(keyboard_node);
            }
            else if (node_type == "MOUSEINPUT")
            {
                MouseInputNode* mouse_node = new MouseInputNode(Input, 
                                                        std::stoi(GetLineBetween(line, "[KEY=", "]")),
                                                        GetLineBetween(line, "[INPUTTYPE=", "]"));
                all_script_nodes.push_back(mouse_node);
            }
            else if (node_type == "APPLYFORCE")
            {
                if (_Node->NodeType != Node::Type::PHYSICSBODY)
                {
                    ScriptError("APPLYFORCE ScriptNode only works with PhysicsBody Node.", file_path);
                    return;
                }
                ApplyForceNode* apply_force_node = new ApplyForceNode((PhysicsBody*)_Node, Vector2(std::stof(GetLineBetween(line, "[FORCEX=", "]")), std::stof(GetLineBetween(line, "[FORCEY=", "]"))));

                all_script_nodes.push_back(apply_force_node);

                if (GetLineBetween(line, "[CONNECTEDID=", "]") != "NULL")
                    all_script_nodes[std::stoi(GetLineBetween(line, "[CONNECTEDID=", "]"))]->ConnectedNodes.push_back(apply_force_node); 
            }
            else if (node_type == "SETVELOCITY")
            {
                if (_Node->NodeType != Node::Type::PHYSICSBODY)
                {
                    ScriptError("APPLYFORCE ScriptNode only works with PhysicsBody Node.", file_path);
                    return;
                }
                SetVelocityNode* set_velocity_node = new SetVelocityNode((PhysicsBody*)_Node, Vector2(std::stof(GetLineBetween(line, "[VELOCITYX=", "]")), std::stof(GetLineBetween(line, "[VELOCITYY=", "]"))));

                all_script_nodes.push_back(set_velocity_node);

                if (GetLineBetween(line, "[CONNECTEDID=", "]") != "NULL")
                    all_script_nodes[std::stoi(GetLineBetween(line, "[CONNECTEDID=", "]"))]->ConnectedNodes.push_back(set_velocity_node); 
            }
        }
        script_file.close();
    }
}

void SetNode(Node* _Node, const std::string& Line, InputManager* Input)
{
    if (IsLineExist(Line, "[POSITION(")) 
    {
        std::string pose = GetLineBetween(Line, "[POSITION(", ")]");
        _Node->Position = Vector2(std::stof(GetLineBetween(pose, 0, ",")), std::stof(GetLineBetween(pose, ",")));
    }
    if (IsLineExist(Line, "[SIZE("))
    {
        std::string size = GetLineBetween(Line, "[SIZE(", ")]");
        _Node->Size = Vector2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ",")));
    }
    if (IsLineExist(Line, "[ANGLE("))
    {
        std::string angle = GetLineBetween(Line, "[ANGLE(", ")]");
        _Node->Angle = std::stof(angle);
    }
    
    _Node->Name = GetLineBetween(Line, "[NAME=", "]");

    _Node->SetID();

    SetScript(_Node, Line, Input);
}

void SetChild(Node* _Node, std::vector<Node*> AllNodes, const std::string& Line, int IndexOffset)
{
    if (IsLineExist(Line, "[CHILDINDEX="))
    {
        std::string child_index = GetLineBetween(Line, "[CHILDINDEX=(", ")]");
        if (IsLineExist(child_index, ","))
        {
            for (int i = 0; i < static_cast<int>(child_index.size()); i++)
            {
                if (child_index[i] != ',')
                {
                    int cti = child_index[i] - '0';
                    _Node->AddChild(AllNodes[cti + IndexOffset]);
                }
            }
        }
        else
        {
            _Node->AddChild(AllNodes[std::stoi(child_index) + IndexOffset]);
        }
    }
}