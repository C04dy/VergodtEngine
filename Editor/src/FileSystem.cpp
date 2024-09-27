#include "FileSystem.h"
#include "imgui.h"
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include "StringFunctions.h"

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{
    m_FileDatas.clear();
}

void FileSystem::FileSystemSpace()
{
    ImGui::Begin("FileSystem");

    for (int i = 0; i < static_cast<int>(m_FileDatas.size()); i++)
    {
        ImGui::PushID(i);

        ImGui::Button(m_FileDatas[i].FilePath.c_str(), ImVec2(100, 100));
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("FILE", &m_FileDatas[i].FilePath, sizeof(m_FileDatas[i].FilePath), ImGuiCond_Once);
            ImGui::EndDragDropSource();
        }
        ImGui::SameLine();

        ImGui::PopID();
    }
    

    ImGui::End();
}

void FileSystem::SetProjectDirectory(const std::string &ProjectDirectory)
{
    m_ProjectDirectory = ProjectDirectory;

    for (const auto& entry : std::filesystem::directory_iterator(m_ProjectDirectory)) {
        std::filesystem::path outfilename = entry.path();
        std::string outfilename_str = outfilename.string();
        
        m_FileDatas.push_back(FileData());

        m_FileDatas[m_FileDatas.size() - 1].FilePath = RemoveFromLine(outfilename_str, m_ProjectDirectory);
    }    
}