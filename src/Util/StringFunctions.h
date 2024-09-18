#ifndef THIS_FILE_H
#define THIS_FILE_H

#include <algorithm>
#include <iostream>
#include <string>

template <typename T>
void Log(T Log);
bool IsLineExist(const std::string& Text, const std::string& WhichLine);
std::string GetLineBetween(const std::string& Text, const std::string& Start, const std::string& Finish);
std::string GetLineBetween(const std::string& Text, int Start, const std::string& Finish);
std::string GetLineBetween(const std::string& Text, const std::string& Start);
std::string GetLineBetweenTillLast(const std::string& Text, const std::string& Last);
std::string GetLineBetweenAfterLast(const std::string& Text, const std::string& Last);
std::string RemoveFromLine(const std::string& Text, const std::string& TextToRemove);
int HowMuchIsInsideString(const std::string& Text, char WhichChar);
int KeyToInt(const std::string& Key);
int InputTypeToInt(const std::string& Type);
std::string RemoveSpaceAndUpperCase(std::string Text);

#endif