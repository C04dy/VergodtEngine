#include "StringFunctions.h"

template <typename T>
void Log(T log)
{
    std::cout << log << "\n";
}

bool IsLineExist(const std::string& Text, const std::string& WhichLine)
{
    return Text.find(WhichLine) != std::string::npos;
}

std::string GetLineBetween(const std::string& Text, const std::string& Start, const std::string& Finish)
{
    if (IsLineExist(Text, Start))
        return Text.substr( Text.find( Start ) + ( Start.length() ) , Text.find( Finish, Text.find(Start) ) - ( Text.find(Start) + (Start.length() ) ) );
    
    return "None";
}

std::string GetLineBetween(const std::string& Text, int Start, const std::string& Finish)
{
    return Text.substr(Start, Text.find(Finish) - Start);
}

std::string GetLineBetween(const std::string& Text, int Size)
{
    return Text.substr(0, Size);
}

std::string GetLineBetween(const std::string& Text, const std::string& Start)
{
    return Text.substr(Text.find(Start) + (Start.length()), Text.length() - Text.find(Start));
}

std::string GetLineBetweenTillLast(const std::string& Text, const std::string& Last)
{
    return Text.substr(0, Text.find_last_of(Last));
}

std::string GetLineBetweenAfterLast(const std::string& Text, const std::string& Last)
{
    return Text.substr(Text.find_last_of(Last) + (Last.length()), Text.length() - Text.find_last_of(Last));
}

std::string RemoveFromLine(const std::string& Text, const std::string& TextToRemove)
{
    std::string result = Text;
    size_t pos = result.find(TextToRemove);

    while (pos != std::string::npos)
    {
        result.erase(pos, TextToRemove.length());
        pos = result.find(TextToRemove);
    }

    return result;
}

int HowMuchIsInsideString(const std::string& Text, char WhichChar)
{
    int count = 0;
 
    for (size_t i = 0; i < Text.length();i++)
        if (Text[i] == WhichChar)
            count++;

    return count;
}

int KeyToInt(const std::string& Key)
{
    if (Key == "A")
        return 1;
    return 0;
}

int InputTypeToInt(const std::string& Type)
{
    if (Type == "ISMOUSEKEYJUSTPRESSED" || Type == "ISKEYJUSTPRESSED")
        return 1;
    if(Type == "ISMOUSEKEYJUSTRELEASED" || Type == "ISKEYJUSTRELEASED")
        return 2;
    if(Type == "ISMOUSEKEYPRESSED" || Type == "ISKEYPRESSED")
        return 3;
    if(Type == "ISMOUSEKEYNOTPRESSED" || Type == "ISKEYNOTPRESSED")
        return 4;
    return 0;
}

std::string RemoveSpaceAndUpperCase(std::string Text)
{
    Text.erase(remove_if(Text.begin(), Text.end(), isspace), Text.end());
    std::transform(Text.begin(), Text.end(), Text.begin(), ::toupper);
    return Text;
}