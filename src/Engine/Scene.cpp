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

                node->NodeType = Node::Type::NODE;

                SetNode(node, line, m_Input);
 
                SetChild(node, m_Nodes, line, index_offset);

                m_Nodes.push_back(node);
            }
            else if (current_node_type == "CAMERA")
            {
                m_MainCamera.NodeType = Node::Type::CAM;
                SetNode(&m_MainCamera, line, m_Input);

                m_Nodes.push_back(&m_MainCamera);
            }
            else if (current_node_type == "SPRITE")
            {
                std::string asset_file_path = GetLineBetween(line, "[ASSET=", "]");

                Sprite* sprite = new Sprite();

                sprite->NodeType = Node::Type::SPRITE;

                sprite->InitSprite(asset_file_path, &m_MainCamera, m_Renderer);

                SetNode(sprite, line, m_Input);

                SetChild(sprite, m_Nodes, line, index_offset);

                m_Nodes.push_back(sprite);
            }
            else if (current_node_type == "PHYSICSBODY")
            {
                PhysicsBody* physics_body = new PhysicsBody;

                physics_body->NodeType = Node::Type::PHYSICSBODY;

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

                physics_body->InitPhysicsBody(m_Nodes, m_PhysicsWorld, body_type, friction, density);

                m_Nodes.push_back(physics_body);
            }
            else if (current_node_type == "COLLIDER")
            {
                Collider* collider = new Collider;

                collider->NodeType = Node::Type::COLLIDER;

                SetNode(collider, line, m_Input);

                SetChild(collider, m_Nodes, line, index_offset);

                std::string colllider_type = GetLineBetween(line, "[COLLIDERTYPE=", "]");

                if (colllider_type == "BOX")
                {
                    std::string size = GetLineBetween(line, "[COLLIDERSIZE(", ")");

                    collider->CreateBoxShape(Vector2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ","))));
                }
                else if (colllider_type == "CIRCLE")
                {
                    float radius = std::stof(GetLineBetween(line, "[RADIUS=", "]"));

                    collider->CreateCircleShape(radius);
                }
                else if (colllider_type == "POLYGON")
                {
                    int32 point_count = std::stoi(GetLineBetween(line, "[POINTCOUNT=(", ")]"));

                    Vector2 polygons[point_count];

                    std::string polygon_points = GetLineBetween(line, "[POINTS=", "]");

                    for (int i = 0; i < point_count; i++)
                    {
                        polygons[i].x = std::stof(GetLineBetween(polygon_points, "(", ","));
                        polygons[i].y = std::stof(GetLineBetween(polygon_points, ",", ")"));

                        if (i != point_count)
                            polygon_points.erase(0, GetLineBetween(polygon_points, "(", ")").size() + 3);
                        else
                            polygon_points.erase(0, GetLineBetween(polygon_points, "(", ")").size() + 2);
                    }
                    
                    collider->CreatePolygonShape(polygons, point_count);
                }

                m_Nodes.push_back(collider);
            }
        }
    }
    
    scene_file.close();
}

void Scene::UpdateChilds()
{
    for (Node* node : m_Nodes)
    {
        node->UpdateChild(m_Nodes);
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
        switch (node->NodeType)
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
        switch (node->NodeType)
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
    m_Nodes.clear();
}