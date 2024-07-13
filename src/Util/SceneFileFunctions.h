#include <string>
#include <iostream>

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
    
    n->Transform.Position = Vector2(std::stof(GetLineBetween(pose, 0, ",")), std::stof(GetLineBetween(pose, ",")));
    n->Transform.Size = Vector2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ",")));
    n->Transform.Angle = std::stof(angle);
    n->Name = GetLineBetween(Line, "[NAME=", "]");
}

void SetChild(Node* n, std::vector<Node*> AN, const std::string& Line){
    int ChildSize = std::stoi(GetLineBetween(Line, "[CHILD=", "]"));
    for (int i = 0; i < ChildSize; i++)
    {
        n->AddChild(AN[(AN.size() - 1) - i]);
    } 
}

template <typename T>
void Log(T log)
{
    std::cout << log << "\n";
}