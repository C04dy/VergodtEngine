#include "FileSystem.h"
#include "imgui.h"
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include "StringFunctions.h"
#include "Project.h"
#include <SDL3/SDL.h>

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{
    m_FileDatas.clear();
}

void FileSystem::FileSystemSpace(Project& _Project)
{
    ImGui::Begin("FileSystem");

    if (m_CurrentDirectoryPath != "")
    {
        if (ImGui::Button("..", ImVec2(100, 100)))
        {
            if (HowMuchIsInsideString(m_CurrentDirectoryPath, '\\') > 1)
            {
                m_CurrentDirectoryPath = m_CurrentDirectoryPath.substr(0, m_CurrentDirectoryPath.find(m_CurrentDirectoryName));
                m_CurrentDirectoryName = m_CurrentDirectoryPath.substr(0, m_CurrentDirectoryPath.find_last_of("\\")); 
                m_CurrentDirectoryName = m_CurrentDirectoryName.substr(m_CurrentDirectoryName.find_last_of("\\") + 1);

                std::cout << m_CurrentDirectoryName;
            }
            else
            {
                m_CurrentDirectoryName = "";
                m_CurrentDirectoryPath = "";
            }

            SetDirectory(_Project);
        }
        ImGui::SameLine();
    }

    for (int i = 0; i < static_cast<int>(m_FileDatas.size()); i++)
    {
        ImGui::PushID(i);

        if (m_FileDatas[i].Texture != nullptr)
        {
            ImVec2 uv_min = ImVec2(0.0f, 0.0f);
            ImVec2 uv_max = ImVec2(1.0f, 1.0f);
            ImVec4 tint_col = false ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            ImGui::ImageButton(m_FileDatas[i].FilePath.c_str(), m_FileDatas[i].Texture, ImVec2(100, 100), uv_min, uv_max, tint_col);
        }
        else if (ImGui::Button(m_FileDatas[i].FilePath.c_str(), ImVec2(100, 100)) && m_FileDatas[i].IsFolder)
        {
            m_CurrentDirectoryPath += m_FileDatas[i].FilePath + "\\";
            m_CurrentDirectoryName = m_FileDatas[i].FilePath;
            SetDirectory(_Project);
            ImGui::PopID();
            break;
        }

        if (ImGui::BeginDragDropSource())
        {
            m_DragDropPath = m_CurrentDirectoryPath + m_FileDatas[i].FilePath;
            ImGui::SetDragDropPayload("FILE", &m_DragDropPath, sizeof(m_DragDropPath), ImGuiCond_Once);
            ImGui::EndDragDropSource();
        }
        ImGui::SameLine();

        ImGui::PopID();
    }

    if (ImGui::IsWindowFocused() && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("FileCreate");
    }

    if (ImGui::BeginPopup("FileCreate"))
    {
        if (ImGui::MenuItem("Make Folder"))
        {
            std::filesystem::create_directory(m_ProjectDirectory + m_CurrentDirectoryPath + "Test");
            SetDirectory(_Project);
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}

void FileSystem::SetProjectDirectory(const std::string &ProjectDirectory, Project& _Project)
{
    m_ProjectDirectory = ProjectDirectory;

    SetDirectory(_Project);
}

void FileSystem::CopyDroppedFile(const std::string& CopiedFilePath, Project& _Project)
{
    std::filesystem::path from = CopiedFilePath;
    std::filesystem::path to = m_ProjectDirectory + m_CurrentDirectoryPath;

    std::filesystem::copy(from, to / from.filename(), std::filesystem::copy_options::overwrite_existing);

    SetDirectory(_Project);
}

void FileSystem::SetDirectory(Project& _Project)
{
    m_FileDatas.clear();
    for (const auto& entry : std::filesystem::directory_iterator(m_ProjectDirectory + m_CurrentDirectoryPath)) {
        std::string outfilename = entry.path().string();

        m_FileDatas.push_back(FileData());

        if (std::filesystem::is_directory(entry))
            m_FileDatas[m_FileDatas.size() - 1].IsFolder = true;
        
        if (IsLineExist(outfilename, ".png") || IsLineExist(outfilename, ".jpeg") || IsLineExist(outfilename, ".jpg") || IsLineExist(outfilename, ".webp"))
            _Project.LoadTextureFromFile(outfilename, &m_FileDatas[m_FileDatas.size() - 1].Texture, m_FileDatas[m_FileDatas.size() - 1].TextureWidth, m_FileDatas[m_FileDatas.size() - 1].TextureHeight);
            
        m_FileDatas[m_FileDatas.size() - 1].FilePath = RemoveFromLine(outfilename, m_ProjectDirectory + m_CurrentDirectoryPath);
    } 
}

FileSystem::FileData::~FileData()
{
    if (Texture != nullptr)
        SDL_DestroyTexture(Texture);
}