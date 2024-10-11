#ifndef PROJECT_H
#define PROJECT_H

#pragma once

#include <vector>
#include <string>
#include "imgui.h"

struct SDL_Texture;
struct SDL_Renderer;

struct Node
{
public:
    enum class Type
    {
        NODE = 0,
        SPRITE = 1,
        CAM = 2,
        PHYSICSBODY = 3,
        COLLIDER = 4
    };

    enum class ColliderType
    {
        BOX = 0,
        CIRCLE = 1,
        POLYGONS = 2
    };

    enum class PhysicsbodyType
    {
        DYNAMIC = 0,
        STATIC = 1,
        KINEMATIC = 2,
    };

    struct NodeValue
    {
    public:
        enum class Type
        {
            NULLTYPE = 0,
            STRING = 1,
            INT = 2,
            FLOAT = 3,
            VECTOR2 = 4,
            VECTORARRAY = 5
        };

        NodeValue(void* _Value, Type _Type);

        NodeValue(void* _Value, Type _Type, const std::vector<std::string>& _ComboValues);

        NodeValue(std::vector<void*> _Value, Type _Type);

        ~NodeValue();
    public:
        void* Value = nullptr;

        std::vector<void*> VectorValues;
        Type VectorValueType = Type::NULLTYPE;

        Type ValueType = Type::NULLTYPE;

        std::vector<std::string> ComboValues;
    };
public:
    Node(ImVec2 _Position = ImVec2(0, 0), Type _NodeType = Type::NODE);

    bool isEqual(const Node& Other) const { return (Other.ID == ID); }

    bool operator==(const Node& Other) { return isEqual(Other); }
    
    bool operator==(const Node* Other) { return isEqual(*Other); }
private:
    static int s_IDs;
public:
    int ID;
    
    Type NodeType = Type::NODE;

    std::string Name = "NodeName", Script = "None";

    ImVec2 Position = ImVec2(0, 0), Size = ImVec2(1, 1);

    float Angle = 0;

    std::vector<NodeValue*> NodeValues;

    bool IsChild = false;

    std::vector<int> ChildIDs;
    bool TreeOpen = false; // i didn't want to make a variable like this but i have no other solution
    
    int ParentID = -1;

    SDL_Texture* Texture = nullptr;

    int TextureWidth, TextureHeight;

    int ColliderCount = 0;
};

std::string CreateFileOpenDialog(const std::vector<std::string>& FileTypes, const std::vector<std::string>& FileExtensions);
std::string CreateFileSaveDialog(const std::vector<std::string>& FileTypes, const std::vector<std::string>& FileExtensions);

class FileSystem;

class Project
{
public:
    Project();
    ~Project();

    void LoadSceneFile(const std::string& FilePath);

    void LoadTextureFromFile(const std::string& FilePath, SDL_Texture** Texture, int& Width, int& Height);

    void SetRenderer(SDL_Renderer* Renderer) { m_Renderer = Renderer; }
    
    bool SaveSceneFile(FileSystem& _FileSystem);

    bool InitilizeProject();

    std::string GetProjectLocation() { return m_ProjectLocation; }

    std::string GetProjectFile() { return m_ProjectFile; }
private:
    void LoadProjectFile(const std::string& FilePath);
public:
    bool SavedProject = true;

    std::vector<Node> Nodes;

    bool ProjectInitilized = false;
private:
    std::string m_CurrentScene = "None";

    std::string m_ProjectLocation = "None";

    std::string m_ProjectFile = "None";

    SDL_Renderer* m_Renderer = nullptr;
};

#endif