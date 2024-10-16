#include "Scene.h"
#include <fstream>
#include <algorithm> 
#include <ctime>
#include "Util/SceneFileFunctions.h"

void Scene::AddNodesToScene(const std::string& SceneFilePath)
{
    std::string line;
    std::ifstream scene_file(m_Project->GetProjectLocation() + SceneFilePath);

    if (scene_file.fail())
    {
        std::cout << "Scene File did not found. " << m_Project->GetProjectLocation() + SceneFilePath << "\n";
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
                m_Nodes.emplace_back(new Node);

                m_Nodes[m_Nodes.size() - 1]->NodeType = Node::Type::NODE;

                SetNode(m_Nodes[m_Nodes.size() - 1], line, m_Input, m_Project->GetProjectLocation());
 
                SetChild(m_Nodes[m_Nodes.size() - 1], m_Nodes, line, index_offset);
            }
            else if (current_node_type == "CAMERA")
            {
                m_Nodes.emplace_back(new Camera);
                
                m_Nodes[m_Nodes.size() - 1]->NodeType = Node::Type::CAM;

                SetNode(m_Nodes[m_Nodes.size() - 1], line, m_Input, m_Project->GetProjectLocation());

                m_MainCamera = (Camera*)m_Nodes[m_Nodes.size() - 1];
            }
            else if (current_node_type == "SPRITE")
            {
                m_Nodes.emplace_back(new Sprite);

                std::string asset_file_path = GetLineBetween(line, "[ASSET=", "]");

                m_Nodes[m_Nodes.size() - 1]->NodeType = Node::Type::SPRITE;

                ((Sprite*)m_Nodes[m_Nodes.size() - 1])->InitSprite(m_Project->GetProjectLocation() + asset_file_path, m_Renderer);

                SetNode(m_Nodes[m_Nodes.size() - 1], line, m_Input, m_Project->GetProjectLocation());

                SetChild(m_Nodes[m_Nodes.size() - 1], m_Nodes, line, index_offset);
            }
            else if (current_node_type == "PHYSICSBODY")
            {
                m_Nodes.emplace_back(new PhysicsBody);

                m_Nodes[m_Nodes.size() - 1]->NodeType = Node::Type::PHYSICSBODY;

                SetNode(m_Nodes[m_Nodes.size() - 1], line, m_Input, m_Project->GetProjectLocation());

                SetChild(m_Nodes[m_Nodes.size() - 1], m_Nodes, line, index_offset);

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


								std::vector<int> layers;
								
								std::string which_layers = GetLineBetween(line, "[LAYERS=(", ")]");
								
								int layercount = HowMuchIsInsideString(which_layers, ',');
								
								if (layercount == 0)
								{
										layers.push_back(std::stoi(which_layers));
								}
								else
								{
										for (int i = 0; i < layercount; i++)
										{
												layers.push_back(std::stoi(GetLineBetween(which_layers, 0, ",")));
												which_layers = which_layers.substr(which_layers.find_first_of(',') + 1, which_layers.size() - which_layers.find_first_of(','));
										}
								}


                ((PhysicsBody*)m_Nodes[m_Nodes.size() - 1])->InitPhysicsBody(m_Nodes, m_PhysicsWorldID, body_type, layers, friction, density);
            }
            else if (current_node_type == "COLLIDER")
            {
                m_Nodes.emplace_back(new Collider);

                m_Nodes[m_Nodes.size() - 1]->NodeType = Node::Type::COLLIDER;

                SetNode(m_Nodes[m_Nodes.size() - 1], line, m_Input, m_Project->GetProjectLocation());

                SetChild(m_Nodes[m_Nodes.size() - 1], m_Nodes, line, index_offset);

                std::string colllider_type = GetLineBetween(line, "[COLLIDERTYPE=", "]");

                if (colllider_type == "BOX")
                {
                    std::string size = GetLineBetween(line, "[COLLIDERSIZE(", ")");

                    ((Collider*)m_Nodes[m_Nodes.size() - 1])->CreateBoxShape(Vector2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ","))));
                }
                else if (colllider_type == "CIRCLE")
                {
                    float radius = std::stof(GetLineBetween(line, "[RADIUS=(", ")]"));

                    ((Collider*)m_Nodes[m_Nodes.size() - 1])->CreateCircleShape(radius);
                }
                else if (colllider_type == "POLYGON")
                {
                    int32_t point_count = std::stoi(GetLineBetween(line, "[POINTCOUNT=(", ")]"));

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
                    
                    ((Collider*)m_Nodes[m_Nodes.size() - 1])->CreatePolygonShape(polygons, point_count);
                }
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
    AddNodesToScene(m_Project->GetCurrentScene());

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
            ((Sprite*)node)->DrawImage(m_MainCamera);
            break;
        default:
            break;
        }
    }
}

void Scene::Clean()
{
    for (Node* n : m_Nodes)
    {
        delete n;
        n = nullptr;
    }
    m_Nodes.clear();
    m_MainCamera = nullptr;
}
