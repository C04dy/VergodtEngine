#ifndef APP_H
#define APP_H

#pragma once
#define SDL_MAIN_HANDLED
#include <iostream>
#include <fstream>
#include <vector>

class App
{
public:
    App();
    ~App();

    void InitApp();
    void RunApp();
private:
    std::vector<std::string> m_lines;
    std::ifstream m_scenefile; 
};

#endif