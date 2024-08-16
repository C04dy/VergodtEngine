template <typename T>
void Log(T log)
{
    std::cout << log << "\n";
}

bool IsLineExist(const std::string& Text, const std::string& WhichLine) {
    return Text.find(WhichLine) != std::string::npos;
}

std::string GetLineBetween(const std::string& Text, const std::string& Start, const std::string& Finish){
    if (IsLineExist(Text, Start))
        return Text.substr( Text.find( Start ) + ( Start.length() ) , Text.find( Finish, Text.find(Start) ) - ( Text.find(Start) + (Start.length() ) ) );
    
    return "NULL";
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

int HowMuchIsInsideString(const std::string& s, char c) {
    int count = 0;
 
    for (size_t i = 0; i < s.length();i++)
        if (s[i] == c)
            count++;

    return count;
}