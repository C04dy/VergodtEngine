#include "Project.h"
#include "Util/StringFunctions.h"
#include <fstream>

Project::Project(int argc, char** argv)
{
#ifdef NDEBUG
    m_ProjectLocation = "../Assets/FlappyBird/";
    m_ProjectFile = "FlappyBird.verproj";
#else
    //m_ProjectLocation = "../Assets/FlappyBird/";
    //m_ProjectFile = "FlappyBird.verproj";

    if (argv[1] == nullptr)
        throw std::runtime_error("There is no Project File");

    m_ProjectLocation = argv[1];
    m_ProjectFile = GetLineBetweenAfterLast(m_ProjectLocation, "\\");
    m_ProjectLocation = RemoveFromLine(m_ProjectLocation, m_ProjectFile);
#endif

    std::string line;
    std::ifstream project_file(m_ProjectLocation + m_ProjectFile);
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