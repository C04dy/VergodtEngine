#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#pragma once

#include <string>
#include <vector>

struct SDL_Texture;
class Project;

class FileSystem
{
public:
    struct FileData
    {
        ~FileData();
        
        std::string FilePath = "None";

        SDL_Texture* Texture = nullptr;
        int TextureWidth, TextureHeight;

        bool IsFolder = false;
    };
public:
    FileSystem();
    ~FileSystem();

    void FileSystemSpace(Project& _Project);

    void SetProjectDirectory(const std::string& ProjectDirectory, Project& _Project);

    void CopyDroppedFile(const std::string& CopiedFilePath, Project& _Project);

    void SetDirectory(Project& _Project);
private:
    std::string m_ProjectDirectory = "None";
    std::string m_CurrentDirectoryPath = "";
    std::string m_CurrentDirectoryName = "";
    std::string m_DragDropPath = "";

    std::vector<FileData> m_FileDatas;
};

#endif