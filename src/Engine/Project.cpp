#include "Project.h"
#include "Util/StringFunctions.h"
#include <fstream>

Project::Project(int argc, char** argv)
{
    if (argc == 0)
    {
        m_ProjectLocation = "../Assets/FlappyBird/";
        m_ProjectFile = "FlappyBird.verproj";
    }
    else
    {
        m_ProjectLocation = argv[1];
        m_ProjectFile = GetLineBetweenAfterLast(m_ProjectLocation, "\\");
        m_ProjectLocation = RemoveFromLine(m_ProjectLocation, m_ProjectFile);
    }

    std::string line;
    std::ifstream project_file("../Assets/FlappyBird/FlappyBird.verproj");
    if (project_file.fail())
    {
        std::cout << "Project File did not found.\n";
        return;
    }

    while (std::getline(project_file, line))
    {
        m_CurrentScene = GetLineBetween(line, "CurrentScene=(", ")");

        m_GameName = GetLineBetween(line, "GameName=(", ")");

        m_WindowWidth = std::stof(GetLineBetween(line, "WindowWidth=(", ")"));

        m_WindowHeight = std::stof(GetLineBetween(line, "WindowHeight=(", ")"));
    }

    project_file.close();
}

Project::~Project()
{

}