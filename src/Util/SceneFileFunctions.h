#include <string>
#include <iostream>
#include "StringFunctions.h"

void ScriptError(const std::string& Msg, const std::string& ScriptPath) {
    std::cout << "Script File Error\n" << Msg << '\n' << "Script File Path = " << ScriptPath << '\n';
}

void SetScript(Node* n, const std::string& Line, InputManager* Input) {
    if(IsLineExist(Line, "[SCRIPT=")){
        std::string ln;
        std::string FilePath = GetLineBetween(Line, "[SCRIPT=", "]");
        std::ifstream ScriptFile(FilePath);

        if (ScriptFile.fail()) {
            ScriptError("Script File did not found.", FilePath);
            return;
        }

        StartNode* s = new StartNode;
        UpdateNode* u = new UpdateNode;
        n->Script = new VisualScript;

        n->Script->InitVisualScript(s, u);

        std::vector<ScriptingNode*> ALLSCRIPNODES;

        while (std::getline(ScriptFile, ln)){
            std::string NodeType = GetLineBetween(ln, "[TYPE=", "]");

            if (NodeType == "START"){
                ALLSCRIPNODES.push_back(s);
            } else if (NodeType == "UPDATE"){
                ALLSCRIPNODES.push_back(u);
            } else if (NodeType == "PRINT"){
                PrintNode* p = new PrintNode;

                p->Message = GetLineBetween(ln, "(", ")");

                ALLSCRIPNODES.push_back(p);

                if (GetLineBetween(ln, "[CONNECTEDID=", "]") != "NULL")
                    ALLSCRIPNODES[std::stoi(GetLineBetween(ln, "[CONNECTEDID=", "]"))]->ConnectedNodes.push_back(p); 
            } else if (NodeType == "KEYBOARDINPUT"){
                KeyboardInputNode* k = new KeyboardInputNode(Input, 
                                                            GetLineBetween(ln, "[KEY=", "]"),
                                                            GetLineBetween(ln, "[INPUTTYPE=", "]"));

                ALLSCRIPNODES.push_back(k);
            } else if (NodeType == "MOUSEINPUT"){
                MouseInputNode* k = new MouseInputNode(Input, 
                                                        std::stoi(GetLineBetween(ln, "[KEY=", "]")),
                                                        GetLineBetween(ln, "[INPUTTYPE=", "]"));

                ALLSCRIPNODES.push_back(k);
            } else if(NodeType == "APPLYFORCE"){
                if (n->Type != NodeType::PHYSICSBODY) {
                    std::cout << (int)n->Type;
                    ScriptError("APPLYFORCE ScriptNode only works with PhysicsBody Node.", FilePath);
                    return;
                }
                ApplyForceNode* a = new ApplyForceNode((PhysicsBody*)n, Vector2(std::stof(GetLineBetween(ln, "[FORCEX=", "]")), std::stof(GetLineBetween(ln, "[FORCEY=", "]"))));

                ALLSCRIPNODES.push_back(a);

                if (GetLineBetween(ln, "[CONNECTEDID=", "]") != "NULL")
                    ALLSCRIPNODES[std::stoi(GetLineBetween(ln, "[CONNECTEDID=", "]"))]->ConnectedNodes.push_back(a); 
            }
        }

        ScriptFile.close();
    }
}

void SetNode(Node* n, const std::string& Line, InputManager* i) {
    if (IsLineExist(Line, "[POSITION(")) {
        std::string pose = GetLineBetween(Line, "[POSITION(", ")]");
        n->Position = Vector2(std::stof(GetLineBetween(pose, 0, ",")), std::stof(GetLineBetween(pose, ",")));
    }
    if (IsLineExist(Line, "[SIZE(")) {
        std::string size = GetLineBetween(Line, "[SIZE(", ")]");
        n->Size = Vector2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ",")));
    }
    if (IsLineExist(Line, "[ANGLE(")) {
        std::string angle = GetLineBetween(Line, "[ANGLE(", ")]");
        n->Angle = std::stof(angle);
    }
    
    n->Name = GetLineBetween(Line, "[NAME=", "]");

    SetScript(n, Line, i);
}

void SetChild(Node* n, std::vector<Node*> AN, const std::string& Line, int IndexOffset){
    if (IsLineExist(Line, "[CHILDINDEX=")) {
        std::string ci = GetLineBetween(Line, "[CHILDINDEX=(", ")]");
        if (ci.find(",") != std::string::npos) {
            for (size_t i = 0; i < ci.size(); i++) {
                if (ci[i] != ',') {
                    int cti = ci[i] - '0';
                    n->AddChild(AN[cti + IndexOffset]);
                }
            }
        } else {
            n->AddChild(AN[std::stoi(ci) + IndexOffset]);
        }
    }
}