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

void SetNode(Node* n, const std::string& Line){
    std::string pose = GetLineBetween(Line, "[POSITION(", ")]");
    std::string size = GetLineBetween(Line, "[SIZE(", ")]");
    std::string angle = GetLineBetween(Line, "[ANGLE(", ")]");
    
    n->Position = Vector2(std::stof(GetLineBetween(pose, 0, ",")), std::stof(GetLineBetween(pose, ",")));
    n->Size = Vector2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ",")));
    n->Angle = std::stof(angle);
    n->Name = GetLineBetween(Line, "[NAME=", "]");
}

int HowMuchIsInsideString(const std::string& s, char c) {
    int count = 0;
 
    for (size_t i = 0; i < s.length();i++)
        if (s[i] == c)
            count++;

    return count;
}

void SetChild(Node* n, std::vector<Node*> AN, const std::string& Line, int IndexOffset){
    if (GetLineBetween(Line, "[CHILD=", "]") != "0") {
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