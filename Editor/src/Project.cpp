#include "Project.h"
#include <iostream>
#include <fstream>
#include "StringFunctions.h"
#include "nfd.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "FileSystem.h"

int Node::s_IDs = 0;


#if _WIN32

#define SLASHCHAR "\\"

#else

#define SLASHCHAR "/"

#endif

Node::Node(ImVec2 _Position, Type _NodeType)
{
    Position = _Position;
    NodeType = _NodeType;

    switch (NodeType)
    {
    case Node::Type::SPRITE:
        NodeValues.push_back(new Node::NodeValue(new std::string("None"), Node::NodeValue::Type::STRING));
        break;
    case Node::Type::COLLIDER:
        NodeValues.push_back(new Node::NodeValue(new Node::ColliderType(Node::ColliderType::BOX), Node::NodeValue::Type::INT, { "Box", "Circle", "Polygon" }));
        NodeValues.push_back(new Node::NodeValue(new ImVec2(10.0f, 10.0f), Node::NodeValue::Type::VECTOR2));
        break;
    case Node::Type::PHYSICSBODY:
        NodeValues.push_back(new Node::NodeValue(new Node::PhysicsbodyType(Node::PhysicsbodyType::DYNAMIC), Node::NodeValue::Type::INT, { "Dynamic", "Static", "Kinematic" }));
        NodeValues.push_back(new Node::NodeValue(new float(0.3f), Node::NodeValue::Type::FLOAT));
        NodeValues.push_back(new Node::NodeValue(new float(1.0f), Node::NodeValue::Type::FLOAT));
        break;
    default:
        break;
    }

    ID = s_IDs;
    s_IDs += 1;
}

Node::NodeValue::NodeValue(void* _Value, Type _Type)
{
    Value = _Value;
    ValueType = _Type;
}

Node::NodeValue::NodeValue(void* _Value, Type _Type, const std::vector<std::string>& _ComboValues)
{
    Value = _Value;
    ValueType = _Type;

    ComboValues = _ComboValues;
}

Node::NodeValue::NodeValue(std::vector<void*> _Value, Type _Type)
{
    VectorValues = _Value;
    ValueType = Type::VECTORARRAY;
}

Node::NodeValue::~NodeValue()
{
    switch (ValueType)
    {
    case Type::NULLTYPE:
        break;
    case Type::FLOAT:
        delete (float*)Value;
        break;
    case Type::INT:
        delete (int*)Value;
        break;
    case Type::STRING:
        delete (std::string*)Value;
        break;
    case Type::VECTOR2:
        delete (ImVec2*)Value;
        break;
    case Type::VECTORARRAY:
        for (int i = 0; i < static_cast<int>(VectorValues.size()); i++)
        {
            switch (VectorValueType)
            {
            case Type::FLOAT:
                delete (float*)VectorValues[i];
                break;
            case Type::INT:
                delete (int*)VectorValues[i];
                break;
            case Type::STRING:
                delete (std::string*)VectorValues[i];
                break;
            case Type::VECTOR2:
                delete (ImVec2*)VectorValues[i];
                break;
            default:
                break;
            }

            VectorValues[i] = nullptr;
        }
        VectorValues.clear();
        break;
    }

    Value = nullptr;
    ComboValues.clear();
}

std::string CreateFileOpenDialog(const std::vector<std::string>& FileTypes, const std::vector<std::string>& FileExtensions)
{
    nfdu8char_t* out_path;

    nfdu8filteritem_t filters[FileTypes.size()];

    for (int i = 0; i < static_cast<int>(FileTypes.size()); i++)
    {
        filters[i] = { FileTypes[i].c_str(), FileExtensions[i].c_str() };
    }

    nfdopendialogu8args_t args = { 0 };
    args.filterList = filters;
    args.filterCount = FileTypes.size();
    nfdresult_t result = NFD_OpenDialogU8_With(&out_path, &args);
    if (result == NFD_OKAY)
    {
        std::string path = out_path;
        NFD_FreePathU8(out_path);
        return path;
    }
    else if (result != NFD_CANCEL)
    {
        std::cout << "Error file did not opened: " << NFD_GetError() << '\n';
    }
    return "None";
}

std::string CreateFileSaveDialog(const std::vector<std::string> &FileTypes, const std::vector<std::string> &FileExtensions)
{
    nfdchar_t* save_path;

    nfdu8filteritem_t filters[FileTypes.size()];

    for (int i = 0; i < static_cast<int>(FileTypes.size()); i++)
    {
        filters[i] = { FileTypes[i].c_str(), FileExtensions[i].c_str() };
    }

    nfdresult_t result = NFD_SaveDialog(&save_path, filters, FileTypes.size(), NULL, ("File." + FileExtensions[0]).c_str());
    if (result == NFD_OKAY)
    {
        std::string path = save_path;
        NFD_FreePath(save_path);

        std::ofstream out_file(path);
        out_file.close();

        return path;
    }
    else if (result != NFD_CANCEL)
    {
        std::cout << "Error file did not created: " << NFD_GetError() << '\n';
    }
    return "None";
}

Project::Project()
{

}

Project::~Project()
{
    for (int i = 0; i < static_cast<int>(Nodes.size()); i++)
    {
        for (int j = 0; j < static_cast<int>(Nodes[i].NodeValues.size()); j++)
        {
            delete Nodes[i].NodeValues[j];
            Nodes[i].NodeValues[j] = nullptr;
        }
        if (Nodes[i].Texture != nullptr)
            SDL_DestroyTexture(Nodes[i].Texture);
    }
}

static std::string GetNodeTypeAsString(Node::Type NodeType)
{
    switch (NodeType)
    {
    case Node::Type::NODE:
        return "NODE";
        break;
    case Node::Type::SPRITE:
        return "SPRITE";
        break;
    case Node::Type::CAM:
        return "CAMERA";
        break;
    case Node::Type::PHYSICSBODY:
        return "PHYSICSBODY";
        break;
    case Node::Type::COLLIDER:
        return "COLLIDER";
        break;
    }
    return "None";
}

static std::string GetPhysicsbodyTypeAsString(Node::PhysicsbodyType PhysicsbodyNodeType)
{
    switch (PhysicsbodyNodeType)
    {
    case Node::PhysicsbodyType::DYNAMIC:
        return "DYNAMIC";
        break;
    case Node::PhysicsbodyType::KINEMATIC:
        return "KINEMATIC";
        break;
    case Node::PhysicsbodyType::STATIC:
        return "STATIC";
        break;
    }
    return "None";
}

static std::string GetColliderTypeAsString(Node::ColliderType ColliderNodeType)
{
    switch (ColliderNodeType)
    {
    case Node::ColliderType::BOX:
        return "BOX";
        break;
    case Node::ColliderType::CIRCLE:
        return "CIRCLE";
        break;
    case Node::ColliderType::POLYGONS:
        return "POLYGON";
        break;
    }
    return "None";
}

static void SaveNodes(std::string& Line, const std::vector<Node>& Nodes, std::vector<Node>& AllNodes, std::vector<Node*>& SavedNodes, bool IsChild = false)
{
    std::locale::global(std::locale("C"));
    for (int i = 0; i < static_cast<int>(Nodes.size()); i++)
    {
        if (Nodes[i].IsChild && !IsChild)
            continue;
        if (Nodes[i].ChildIDs.size() > 0)
        {
            std::vector<Node> Childs(Nodes[i].ChildIDs.size());

            for (int j = 0; j < static_cast<int>(Nodes[i].ChildIDs.size()); j++)
                for (int x = 0; x < static_cast<int>(AllNodes.size()); x++)
                    if (AllNodes[x].ID == Nodes[i].ChildIDs[j])
                        Childs[j] = AllNodes[x];

            SaveNodes(Line, Childs, AllNodes, SavedNodes, true);
        }
        for (int j = 0; j < static_cast<int>(AllNodes.size()); j++)
            if (AllNodes[j].ID == Nodes[i].ID)
                SavedNodes.emplace_back(&AllNodes[j]);

        Line += "[NODETYPE=";
        Line += GetNodeTypeAsString(Nodes[i].NodeType);
        Line += "] ";

        Line += "[NAME=";
        Line += Nodes[i].Name;
        Line += "] ";

        if (Nodes[i].Position.x != 0.0f || Nodes[i].Position.y != 0.0f)
        {
            Line += "[POSITION(";
            Line += std::to_string(Nodes[i].Position.x);
            Line += ',';
            Line += std::to_string(Nodes[i].Position.y);
            Line += ")] ";
        }
        if (Nodes[i].Size.x != 1.0f || Nodes[i].Size.y != 1.0f)
        {
            Line += "[SIZE(";
            Line += std::to_string(Nodes[i].Size.x);
            Line += ',';
            Line += std::to_string(Nodes[i].Size.y);
            Line += ")] ";
        }
        if (Nodes[i].Angle != 0.0f)
        {
            Line += "[ANGLE(";
            Line += std::to_string(Nodes[i].Angle);
            Line += ")] ";
        }
        if (Nodes[i].ChildIDs.size() > 0)
        {
            Line += "[CHILDINDEX=(";
            for (int j = 0; j < static_cast<int>(Nodes[i].ChildIDs.size()); j++)
            {
                for (int x = 0; x < static_cast<int>(SavedNodes.size()); x++)
                {
                    if (Nodes[i].ChildIDs[j] == SavedNodes[x]->ID)
                    {
                        Line += std::to_string(x);
                        Line += ',';
                    }
                }
            }
            Line.erase(Line.size() - 1, Line.size());
            Line += ")] ";
        }
        if (Nodes[i].Script != "None")
        {
            Line += "[SCRIPT=";
            Line += Nodes[i].Script;
            Line += "] ";
        }

        switch (Nodes[i].NodeType)
        {
        case Node::Type::SPRITE:
            Line += "[ASSET=";
            Line += *(std::string*)Nodes[i].NodeValues[0]->Value;
            Line += "] ";
            break;
        case Node::Type::PHYSICSBODY:
            Line += "[PHYSICSTYPE=";
            Line += GetPhysicsbodyTypeAsString(*(Node::PhysicsbodyType*)Nodes[i].NodeValues[0]->Value);
            Line += "] ";

            Line += "[FRICTION=";
            Line += std::to_string(*(float*)Nodes[i].NodeValues[1]->Value);
            Line += "] ";

            Line += "[DENSITY=";
            Line += std::to_string(*(float*)Nodes[i].NodeValues[2]->Value);
            Line += "] ";
            break;
        case Node::Type::COLLIDER:
            Line += "[COLLIDERTYPE=";
            Line += GetColliderTypeAsString(*(Node::ColliderType*)Nodes[i].NodeValues[0]->Value);
            Line += "] ";

            switch (*(Node::ColliderType*)Nodes[i].NodeValues[0]->Value)
            {
            case Node::ColliderType::BOX:
                Line += "[COLLIDERSIZE(";
                Line += std::to_string(((ImVec2*)Nodes[i].NodeValues[1]->Value)->x);
                Line += ",";
                Line += std::to_string(((ImVec2*)Nodes[i].NodeValues[1]->Value)->y);
                Line += ")] ";
                break;
            case Node::ColliderType::CIRCLE:
                Line += "[RADIUS=(";
                Line += std::to_string((*(float*)Nodes[i].NodeValues[1]->Value));
                Line += ")] ";
                break;
            case Node::ColliderType::POLYGONS:
                Line += "[POINTS=";
                for (int j = 0; j < static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()); j++)
                {
                    Line += "(";
                    Line += std::to_string( (*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).x );
                    Line += ",";
                    Line += std::to_string( (*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).y );
                    Line += "),";
                }
                Line.erase(Line.size() - 1, Line.size());
                Line += "] ";

                Line += "[POINTCOUNT=(";
                Line += std::to_string(static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()));
                Line += ")] ";
                break;
            }
            break;
        default:
            break;
        }
        Line += '\n';
    }

    if (!IsChild && Line.size() > 0)
        Line.erase(Line.size() - 2, Line.size());
}

bool Project::SaveSceneFile(FileSystem& _FileSystem)
{
    if (m_CurrentScene == "None")
    {
        std::string scene_path = CreateFileSaveDialog({ "VergodtEngine Scene File" }, { "vscene" });
        if (scene_path == "None")
            return false;
        m_CurrentScene = scene_path;
        _FileSystem.SetDirectory(*this);
    }

    std::string line;

    std::vector<Node*> saved_nodes;
    saved_nodes.reserve(Nodes.size());
    SaveNodes(line, Nodes, Nodes, saved_nodes);
    saved_nodes.clear();

    // this will be fixed

    std::ofstream out_file(m_ProjectLocation + m_ProjectFile);
    out_file << "CurrentScene=(";
    out_file << m_CurrentScene;
    out_file << ") ";
    out_file << "GameName=(" + RemoveFromLine(m_ProjectFile, ".verproj") + ") ";
    out_file << "WindowWidth=(1280) WindowHeight=(720)";
    out_file.close();

    std::cout << m_ProjectLocation;

    std::ofstream write_file(m_ProjectLocation + m_CurrentScene);
    write_file << line;
    return true;
}

bool Project::InitilizeProject()
{
    ImGui::Begin("Project List");

    if (ImGui::Button("Select Project"))
    {
        std::string project_location = CreateFileOpenDialog({ "VergodtEngine Project File" }, { "verproj" });

        if (project_location != "None")
        {
            LoadProjectFile(project_location);

            ProjectInitilized = true;
            ImGui::End();
            return true;
        }
    }

    if (ImGui::Button("Create Project"))
    {
        std::string project_location = CreateFileSaveDialog({ "VergodtEngine Project File" }, { "verproj" });

        std::ofstream out_file(project_location);
        out_file << "GameName=(" + RemoveFromLine(GetLineBetweenAfterLast(project_location, SLASHCHAR), ".verproj") + ") ";
        out_file << "WindowWidth=(1280) WindowHeight=(720)";
        out_file.close();
        
        if (project_location != "None")
        {
            LoadProjectFile(project_location);

            ProjectInitilized = true;
            ImGui::End();
            return true;
        }
    }

    ImGui::End();
    return false;
}

void Project::LoadProjectFile(const std::string& FilePath)
{
    m_ProjectLocation = GetLineBetweenTillLast(FilePath, SLASHCHAR);
    m_ProjectLocation += SLASHCHAR;
    m_ProjectFile = GetLineBetweenAfterLast(FilePath, SLASHCHAR);

    std::string line;
    std::ifstream project_file(FilePath);
    if (project_file.fail())
    {
        std::cout << FilePath << '\n';
        throw std::runtime_error("Project File did not found.\n");
    }

    while (std::getline(project_file, line))
    {
        m_CurrentScene = GetLineBetween(line, "CurrentScene=(", ")");
    }

    project_file.close();

    if (m_CurrentScene != "None")
    {
        LoadSceneFile(m_CurrentScene);   
    }
}

void Project::LoadTextureFromFile(const std::string& FilePath, SDL_Texture** Texture, int& Width, int& Height)
{
    SDL_Surface* surface = IMG_Load(FilePath.c_str());

    if (surface == nullptr)
    {
        std::cout << "Failed to load texture: " << FilePath.c_str() << '\n';
        return;
    }

    if (*Texture != nullptr)
        SDL_DestroyTexture(*Texture);

    *Texture = SDL_CreateTextureFromSurface(m_Renderer, surface);

    if (*Texture == nullptr)
    {
        std::cout << "Failed to create texture from surface\n";
        return;
    }

    Width = surface->w;
    Height = surface->h;

    SDL_DestroySurface(surface);
}

void Project::LoadSceneFile(const std::string& FilePath)
{
    std::string line;
    std::ifstream scene_file(m_ProjectLocation + FilePath);

    if (scene_file.fail())
    {
        std::cout << "Scene File did not found: " << FilePath << '\n';
        return;
    }
    
    m_CurrentScene = FilePath;

    int line_count = 0;
    Nodes.clear();
    while (std::getline(scene_file, line))
    {
        if (line[0] != '#')
        {
            line_count += 1;
            Nodes.push_back(Node());

            std::string type = GetLineBetween(line, "[NODETYPE=", "]");

            if (type == "NODE")
            {
                Nodes[Nodes.size() - 1].NodeType = Node::Type::NODE;
            }
            else if (type == "PHYSICSBODY")
            {
                Nodes[Nodes.size() - 1].NodeType = Node::Type::PHYSICSBODY;

                std::string physicsbody_type = GetLineBetween(line, "[PHYSICSTYPE=", "]");

                if (physicsbody_type == "DYNAMIC")
                {
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::PhysicsbodyType(Node::PhysicsbodyType::DYNAMIC), Node::NodeValue::Type::INT, { "Dynamic", "Static", "Kinematic" }));
                }
                else if (physicsbody_type == "STATIC")
                {
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::PhysicsbodyType(Node::PhysicsbodyType::STATIC), Node::NodeValue::Type::INT, { "Dynamic", "Static", "Kinematic" }));
                }
                else if (physicsbody_type == "KINEMATIC")
                {
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::PhysicsbodyType(Node::PhysicsbodyType::KINEMATIC), Node::NodeValue::Type::INT, { "Dynamic", "Static", "Kinematic" }));
                }

                Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new float(std::stof(GetLineBetween(line, "[FRICTION=", "]"))), Node::NodeValue::Type::FLOAT));
                Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new float(std::stof(GetLineBetween(line, "[DENSITY=", "]"))), Node::NodeValue::Type::FLOAT));
            }
            else if (type == "SPRITE")
            {
                Nodes[Nodes.size() - 1].NodeType = Node::Type::SPRITE;
                if (IsLineExist(line, "[ASSET="))
                {
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new std::string(GetLineBetween(line, "[ASSET=", "]")), Node::NodeValue::Type::STRING));

                    LoadTextureFromFile(m_ProjectLocation + GetLineBetween(line, "[ASSET=", "]"), &Nodes[Nodes.size() - 1].Texture, Nodes[Nodes.size() - 1].TextureWidth, Nodes[Nodes.size() - 1].TextureHeight);
                }
                else
                {
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new std::string("None"), Node::NodeValue::Type::STRING));
                }
            }
            else if (type == "CAMERA")
            {
                Nodes[Nodes.size() - 1].NodeType = Node::Type::CAM;
            }
            else if (type == "COLLIDER")
            {
                Nodes[Nodes.size() - 1].NodeType = Node::Type::COLLIDER;

                std::string collider_type = GetLineBetween(line, "[COLLIDERTYPE=", "]");

                if (collider_type == "BOX")
                {
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::ColliderType(Node::ColliderType::BOX), Node::NodeValue::Type::INT, { "Box", "Circle", "Polygon" }));

                    std::string size = GetLineBetween(line, "[COLLIDERSIZE(", ")]");
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new ImVec2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ","))), Node::NodeValue::Type::VECTOR2));
                }
                else if (collider_type == "CIRCLE")
                {
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::ColliderType(Node::ColliderType::CIRCLE), Node::NodeValue::Type::INT, { "Box", "Circle", "Polygon" }));
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new float(std::stof(GetLineBetween(line, "[RADIUS=(", ")]"))), Node::NodeValue::Type::FLOAT));
                }
                else if (collider_type == "POLYGON")
                {
                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::ColliderType(Node::ColliderType::POLYGONS), Node::NodeValue::Type::INT, { "Box", "Circle", "Polygon" }));

                    std::vector<void*> points;

                    int point_count = std::stoi(GetLineBetween(line, "[POINTCOUNT=(", ")]"));

                    std::string polygon_points = GetLineBetween(line, "[POINTS=", "]");

                    for (int i = 0; i < point_count; i++)
                    {
                        points.push_back(new ImVec2(std::stof(GetLineBetween(polygon_points, "(", ",")), std::stof(GetLineBetween(polygon_points, ",", ")"))));

                        if (i != point_count)
                            polygon_points.erase(0, GetLineBetween(polygon_points, "(", ")").size() + 3);
                        else
                            polygon_points.erase(0, GetLineBetween(polygon_points, "(", ")").size() + 2);
                    }

                    Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(points, Node::NodeValue::Type::VECTOR2));
                }
            }

            if (IsLineExist(line, "[POSITION("))
            {
                std::string pose = GetLineBetween(line, "[POSITION(", ")]");
                Nodes[Nodes.size() - 1].Position = ImVec2(std::stof(GetLineBetween(pose, 0, ",")), std::stof(GetLineBetween(pose, ",")));
            }
            if (IsLineExist(line, "[SIZE("))
            {
                std::string size = GetLineBetween(line, "[SIZE(", ")]");
                Nodes[Nodes.size() - 1].Size = ImVec2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ",")));
            }

            if (IsLineExist(line, "[ANGLE("))
            {
                Nodes[Nodes.size() - 1].Angle = std::stof(GetLineBetween(line, "[ANGLE(", ")]"));
            }
            if (IsLineExist(line, "[SCRIPT="))
            {
                Nodes[Nodes.size() - 1].Script = GetLineBetween(line, "[SCRIPT=", "]");
            }

            if (IsLineExist(line, "[CHILDINDEX="))
            {
                std::string ci = GetLineBetween(line, "[CHILDINDEX=(", ")]");
                if (IsLineExist(ci, ",")) {
                    for (int i = 0; i < static_cast<int>(ci.size()); i++)
                    {
                        if (ci[i] != ',')
                        {
                            Nodes[Nodes.size() - 1].ChildIDs.push_back(ci[i] - '0');
                            Nodes[ci[i] - '0'].IsChild = true;
                            Nodes[ci[i] - '0'].ParentID = Nodes[Nodes.size() - 1].ID;

                            if (Nodes[Nodes.size() - 1].NodeType == Node::Type::PHYSICSBODY && Nodes[ci[i] - '0'].NodeType == Node::Type::COLLIDER)
                                Nodes[Nodes.size() - 1].ColliderCount += 1;
                        }
                    }
                }
                else
                {
                    Nodes[Nodes.size() - 1].ChildIDs.push_back(std::stoi(ci));
                    Nodes[std::stoi(ci)].IsChild = true;
                    Nodes[std::stoi(ci)].ParentID = Nodes[Nodes.size() - 1].ID;

                    if (Nodes[Nodes.size() - 1].NodeType == Node::Type::PHYSICSBODY && Nodes[std::stoi(ci)].NodeType == Node::Type::COLLIDER)
                        Nodes[Nodes.size() - 1].ColliderCount += 1;
                }
            }
            Nodes[Nodes.size() - 1].Name = GetLineBetween(line, "[NAME=", "]");
        }
    }

    scene_file.close();
}