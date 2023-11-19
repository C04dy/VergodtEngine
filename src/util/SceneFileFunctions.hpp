#include <string>
#include <iostream>

std::string GetLineBetween(std::string Text, std::string Start, std::string Finish){
    return Text.substr( Text.find( Start ) + ( Start.length()  ) , Text.find( Finish, Text.find(Start) ) - ( Text.find(Start) + (Start.length()  ) ) );
}