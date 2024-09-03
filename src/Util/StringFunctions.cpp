#include "StringFunctions.h"

template <typename T>
void Log(T log) {
    std::cout << log << "\n";
}

bool IsLineExist(const std::string& Text, const std::string& WhichLine) {
    return Text.find(WhichLine) != std::string::npos;
}

std::string GetLineBetween(const std::string& Text, const std::string& Start, const std::string& Finish) {
    if (IsLineExist(Text, Start))
        return Text.substr( Text.find( Start ) + ( Start.length() ) , Text.find( Finish, Text.find(Start) ) - ( Text.find(Start) + (Start.length() ) ) );
    
    return "NULL";
}

std::string GetLineBetween(const std::string& Text, int Start, const std::string& Finish) {
    return Text.substr( Start , Text.find( Finish ) - Start );
}

std::string GetLineBetween(const std::string& Text, int Size){
    return Text.substr(0 ,Size);
}

std::string GetLineBetween(const std::string& Text, const std::string& Start) {
    return Text.substr( Text.find( Start ) + ( Start.length() ) , Text.length() - Text.find( Start ) );
}

int HowMuchIsInsideString(const std::string& s, char c) {
    int count = 0;
 
    for (size_t i = 0; i < s.length();i++)
        if (s[i] == c)
            count++;

    return count;
}

int KeyToInt(const std::string& Key) {
    if (Key == "A")
        return 1;
    return 0;
}

int InputTypeToInt(const std::string& Type) {
    if(Type == "ISMOUSEKEYJUSTPRESSED" || Type == "ISKEYJUSTPRESSED")
        return 1;
    if(Type == "ISMOUSEKEYJUSTRELEASED" || Type == "ISKEYJUSTRELEASED")
        return 2;
    if(Type == "ISMOUSEKEYPRESSED" || Type == "ISKEYPRESSED")
        return 3;
    if(Type == "ISMOUSEKEYNOTPRESSED" || Type == "ISKEYNOTPRESSED")
        return 4;
    return 0;
}

std::string RemoveSpaceAndUpperCase(const std::string& str) {
    std::string l = str;
    l.erase(remove_if(l.begin(), l.end(), isspace), l.end());
    std::transform(l.begin(), l.end(), l.begin(), ::toupper);
    return l;
}