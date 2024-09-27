#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#pragma once

#include <string>
#include <vector>

class FileSystem
{
public:
    struct FileData
    {
        std::string FilePath = "None";
    };
public:
    FileSystem();
    ~FileSystem();

    void FileSystemSpace();

    void SetProjectDirectory(const std::string& ProjectDirectory);
private:
    std::string m_ProjectDirectory = "None";

    std::vector<FileData> m_FileDatas;
};

#endif