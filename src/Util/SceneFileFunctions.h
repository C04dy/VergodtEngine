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