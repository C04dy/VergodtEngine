#ifndef PROJECT_H
#define PROJECT_H

#pragma once

#include <string>

class Project
{
public:
    Project(int argc = 0, char** argv = nullptr);
    ~Project();

    std::string GetProjectLocation() { return m_ProjectLocation; }
    std::string GetCurrentScene() { return m_CurrentScene; }
    
    std::string GetGameName() { return m_GameName; }
	void SetGameName(const std::string& GameName) { m_GameName = GameName; }

	int GetWindowWidth() { return m_WindowWidth; }
	int GetWindowHeight() { return m_WindowHeight; }

	void SetWindowsWidth(int Width) { m_WindowWidth = Width; }
	void SetWindowsHeight(int Height) { m_WindowHeight = Height; }

private:
    std::string m_GameName = "VergodtEngineGame";

    int m_WindowWidth = 1280, m_WindowHeight = 720;

    std::string m_CurrentScene = "None";

    std::string m_ProjectLocation = "None";

    std::string m_ProjectFile = "None";
};

#endif