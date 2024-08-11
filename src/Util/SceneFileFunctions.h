#include <string>
#include <iostream>

template <typename T>
void Log(T log)
{
    std::cout << log << "\n";
}

std::string GetLineBetween(const std::string& Text, const std::string& Start, const std::string& Finish){
    return Text.substr( Text.find( Start ) + ( Start.length() ) , Text.find( Finish, Text.find(Start) ) - ( Text.find(Start) + (Start.length() ) ) );
}

std::string GetLineBetween(const std::string& Text, int Start, const std::string& Finish){
    return Text.substr( Start , Text.find( Finish ) - Start );
}

std::string GetLineBetween(const std::string& Text, int Size){
    return Text.substr(0 ,Size);
}

std::string GetLineBetween(const std::string& Text, const std::string& Start){
    return Text.substr( Text.find( Start ) + ( Start.length() ) , Text.length() - Text.find( Start ) );
}

bool IsLineExist(const std::string& Text, const std::string& WhichLine) {
    return Text.find(WhichLine) != std::string::npos;
}

void SetScript(Node* n, const std::string& Line, InputManager* Input) {
    if(IsLineExist(Line, "[SCRIPT=")){
        std::string ln;
        std::ifstream ScriptFile(GetLineBetween(Line, "[SCRIPT=", "]"));

        StartNode* s = new StartNode;
        UpdateNode* u = new UpdateNode;
        n->Script = new VisualScript;

        n->Script->InitVisualScript(s, u);

        std::vector<ScriptingNode*> ALLSCRIPNODES;

        while (std::getline(ScriptFile, ln)){
            std::string NodeType = GetLineBetween(ln, "[TYPE=", "]");

            if(NodeType == "START"){
                ALLSCRIPNODES.push_back(s);
            }else if(NodeType == "UPDATE"){
                ALLSCRIPNODES.push_back(u);
            }else if(NodeType == "PRINT"){
                PrintNode* p = new PrintNode;

                p->Message = GetLineBetween(ln, "(", ")");

                ALLSCRIPNODES.push_back(p);

                if(GetLineBetween(ln, "[CONNECTEDID=", "]") != "NULL")
                    ALLSCRIPNODES[std::stoi(GetLineBetween(ln, "[CONNECTEDID=", "]"))]->ConnectedNodes.push_back(p); 
            }else if(NodeType == "KEYBOARDINPUT"){
                KeyboardInputNode* k = new KeyboardInputNode(Input, 
                                                            GetLineBetween(ln, "[KEY=", "]"),
                                                            GetLineBetween(ln, "[INPUTTYPE=", "]"));

                ALLSCRIPNODES.push_back(k);
            }else if(NodeType == "MOUSEINPUT"){
                MouseInputNode* k = new MouseInputNode(Input, 
                                                        std::stoi(GetLineBetween(ln, "[KEY=", "]")),
                                                        GetLineBetween(ln, "[INPUTTYPE=", "]"));

                ALLSCRIPNODES.push_back(k);
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

int HowMuchIsInsideString(const std::string& s, char c) {
    int count = 0;
 
    for (size_t i = 0; i < s.length();i++)
        if (s[i] == c)
            count++;

    return count;
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