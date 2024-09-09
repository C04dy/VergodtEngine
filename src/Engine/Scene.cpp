#include "Scene.h"
#include <fstream>
#include <algorithm> 
#include <ctime>
#include "Util/SceneFileFunctions.h"

void Scene::AddNodesToScene(const std::string& SceneFilePath)
{
    std::string line;
    std::ifstream scene_file(SceneFilePath);

    if (scene_file.fail())
    {
        std::cout << "Scene File did not found.\n";
        return;
    }

    int index_offset = static_cast<int>(m_Nodes.size());

    while (std::getline(scene_file, line))
    {
        if (line.at(0) != '#')
        {
            std::string current_node_type = GetLineBetween(line, "[NODETYPE=", "]");
            if (current_node_type == "NODE")
            {
                Node* node = new Node();

                node->Type = Node::Type::NODE;

                SetNode(node, line, m_Input);
 
                SetChild(node, m_Nodes, line, index_offset);

                m_Nodes.push_back(node);
            }
            else if (current_node_type == "CAMERA")
            {
                m_MainCamera.Type = Node::Type::CAM;
                SetNode(&m_MainCamera, line, m_Input);

                m_Nodes.push_back(&m_MainCamera);
            }
            else if (current_node_type == "SPRITE")
            {
                std::string asset_file_path = GetLineBetween(line, "[ASSET=", "]");

                Sprite* sprite = new Sprite();

                sprite->Type = Node::Type::SPRITE;

                sprite->InitSprite(asset_file_path, &m_MainCamera, m_Renderer);

                SetNode(sprite, line, m_Input);

                SetChild(sprite, m_Nodes, line, index_offset);

                m_Nodes.push_back(sprite);
            }
            else if (current_node_type == "PHYSICSBODY")
            {
                PhysicsBody* physics_body = new PhysicsBody;

                physics_body->Type = Node::Type::PHYSICSBODY;

                SetNode(physics_body, line, m_Input);

                SetChild(physics_body, m_Nodes, line, index_offset);

                b2BodyType body_type = b2BodyType::b2_staticBody;
                
                std::string type = GetLineBetween(line, "[PHYSICSTYPE=", "]");

                if (type == "DYNAMIC")
                {
                    body_type = b2BodyType::b2_dynamicBody;
                }
                else if (type == "STATIC")
                {
                    body_type = b2BodyType::b2_staticBody;
                }
                else if (type == "KINEMATIC")
                {
                    body_type = b2BodyType::b2_kinematicBody;
                }

                float friction = std::stof(GetLineBetween(line, "[FRICTION=", "]"));
                float density = std::stof(GetLineBetween(line, "[DENSITY=", "]"));

                std::string colllider_type = GetLineBetween(line ,"[COLLIDER=", "]");

                if (colllider_type == "BOX")
                {
                    std::string size = GetLineBetween(line, "[COLLIDERSIZE(", ")");

                    physics_body->InitPhysicsBodyBox(m_PhysicsWorld, body_type, Vector2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ","))), friction, density);
                }
                else if (colllider_type == "CIRCLE")
                {
                    float radius = std::stof(GetLineBetween(line, "[RADIUS=", "]"));

                    physics_body->InitPhysicsBodyCircle(m_PhysicsWorld, body_type, radius, friction, density);
                }
                else if (colllider_type == "POLYGON")
                {
                    int32 polygon_count = std::stoi(GetLineBetween(line, "[POLYGONCOUNT=", "]"));

                    Vector2 polygons[polygon_count];

                    std::string polygon_sizes = GetLineBetween(line, "[POLYGONS=", "]");

                    for (int i = 0; i < polygon_count; i++)
                    {
                        std::string current_polygon = GetLineBetween(polygon_sizes, std::to_string(i + 1) + "=(", ")");

                        polygons[i].x = std::stof(GetLineBetween(current_polygon, 0, ","));
                        polygons[i].y = std::stof(GetLineBetween(current_polygon, ","));
                    }
                    
                    physics_body->InitPhysicsBodyPolygon(m_PhysicsWorld, body_type, polygons, polygon_count, friction, density);
                }

                m_Nodes.push_back(physics_body);
            }
        }
    }
    scene_file.close();
}

void Scene::UpdateChilds()
{
    for (Node* node : m_Nodes)
    {
        node->UpdateChild();
    }
}

void Scene::Start()
{
    AddNodesToScene("../Assets/FlappyBird/FlappyBird.vscene");
    //AddNodesToScene("../Assets/test.vscene");

    UpdateChilds();

    for (Node* node : m_Nodes)
    {
        if (node->Script != nullptr)
        {
            node->Script->StartScript();
        }
    }
}

void Scene::Update(double Delta)
{
    UpdateChilds();
    for (Node* node : m_Nodes)
    {
        switch (node->Type)
        {
        case Node::Type::PHYSICSBODY:
            ((PhysicsBody*)node)->UpdatePhysicsNode();
            break;
        default:
            break;
        }
        if (node->Script != nullptr)
        {
            node->Script->UpdateScript();
        }
    }
}

void Scene::Draw()
{
    for (Node* node : m_Nodes)
    {
        switch (node->Type)
        {
        case Node::Type::SPRITE:
            ((Sprite*)node)->DrawImage();
            break;
        default:
            break;
        }
    }
}

void Scene::Clean()
{
    for (Node* node : m_Nodes)
    {
        switch (node->Type)
        {
        case Node::Type::SPRITE:
        ((Sprite*)node)->DeleteTexture();
            break;
        case Node::Type::PHYSICSBODY:
        ((PhysicsBody*)node)->DeletePhysicsBody();
            break;
        default:
            break;
        }
    }
    m_Nodes.clear();
}