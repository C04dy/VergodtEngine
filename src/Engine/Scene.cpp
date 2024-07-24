#include "Scene.h"
#include "Scripting/Squirrel/SquirrelBindings.hpp"
#include <fstream>
#include <algorithm> 
#include <ctime>

std::vector<Node*> Scene::AddNodesToScene(const std::string& SceneFilePath) {
    std::string Line;
    std::ifstream SceneFile(SceneFilePath);

    std::vector<Node*> SCRIPTABLENODES;

    int IndexOffset = Nodes.size();

    while (std::getline(SceneFile, Line)){
        if (Line.at(0) != '#') {
            std::string CurNodeType = GetLineBetween(Line, "[NODETYPE=", "]");
            std::size_t ScriptableNodeSize = ScriptableNodes.size();
            if(CurNodeType == "NODE"){
                Node* n = new Node();

                SetNode(n, Line);
 
                SetChild(n, Nodes, Line, IndexOffset);
    
                if (SetNodesScript(Line, n) == true) {
                    ScriptableNodes.push_back(n);
                    n->ScriptIndex = ScriptableNodes.size() - 1;
                }

                Nodes.push_back(n);
            }else if(CurNodeType == "CAMERA"){
                SetNode(&Cam, Line);

                if (SetNodesScript(Line, (Node*)&Cam) == true) {
                    ScriptableNodes.push_back(&Cam);
                    Cam.ScriptIndex = ScriptableNodes.size() - 1;
                }

                Nodes.push_back(&Cam);
            }else if(CurNodeType == "SPRITE"){
                std::string AssetFilePath = GetLineBetween(Line, "[ASSET=", "]");

                Sprite* s = new Sprite();

                s->InitSprite(AssetFilePath, &Cam, Renderer);

                SetNode(s, Line);

                SetChild(s, Nodes, Line, IndexOffset);

                if (SetNodesScript(Line, (Node*)s) == true) {
                    ScriptableNodes.push_back(s);
                    s->ScriptIndex = ScriptableNodes.size() - 1;
                }

                Nodes.push_back(s);
            }else if(CurNodeType == "PHYSICSBODY"){        
                PhysicsBody* p = new PhysicsBody;

                SetNode(p, Line);

                SetChild(p, Nodes, Line, IndexOffset);

                if (SetNodesScript(Line, (Node*)p) == true) {
                    ScriptableNodes.push_back(p);
                    p->ScriptIndex = ScriptableNodes.size() - 1;
                }

                b2BodyType t = b2BodyType::b2_staticBody;
                
                std::string type = GetLineBetween(Line, "[PHYSICSTYPE=", "]");

                if(type == "DYNAMIC"){
                    t = b2BodyType::b2_dynamicBody;
                }else if(type == "STATIC"){
                    t = b2BodyType::b2_staticBody;
                }else if(type == "KINEMATIC"){
                    t = b2BodyType::b2_kinematicBody;
                }

                float fr = std::stof(GetLineBetween(Line, "[FRICTION=", "]"));
                float dn = std::stof(GetLineBetween(Line, "[DENSITY=", "]"));

                std::string colllidertype = GetLineBetween(Line ,"[COLLIDER=", "]");

                if(colllidertype == "BOX"){
                    std::string sz = GetLineBetween(Line, "[COLLIDERSIZE(", ")");

                    p->InitPhysicsBodyBox(PhysicsWorld, t, Vector2(std::stof(GetLineBetween(sz, 0, ",")), std::stof(GetLineBetween(sz, ","))), fr, dn);
                }else if(colllidertype == "CIRCLE"){
                    float rad = std::stof(GetLineBetween(Line, "[RADIUS=", "]"));

                    p->InitPhysicsBodyCircle(PhysicsWorld, t, rad, fr, dn);
                }else if(colllidertype == "POLYGON"){
                    int32 polycount = std::stoi(GetLineBetween(Line, "[POLYGONCOUNT=", "]"));

                    Vector2 poly[polycount];

                    std::string polys = GetLineBetween(Line, "[POLYGONS=", "]");

                    for (int i = 0; i < polycount; i++)
                    {
                        std::string curpoly = GetLineBetween(polys, std::to_string(i + 1) + "=(", ")");

                        poly[i].x = std::stof(GetLineBetween(curpoly, 0, ","));
                        poly[i].y = std::stof(GetLineBetween(curpoly, ","));
                    }
                    
                    p->InitPhysicsBodyPolygon(PhysicsWorld, t, poly, polycount, fr, dn);
                }

                Nodes.push_back(p);
            }

            if (ScriptableNodeSize < ScriptableNodes.size())
                SCRIPTABLENODES.push_back(ScriptableNodes[ScriptableNodes.size() - 1]);
        }
    }
    SceneFile.close();

    return SCRIPTABLENODES;
}

void Scene::RemoveNodeFromScene(Node* n) {
    for (size_t i = 0; i < Nodes.size(); i++) {
        if (Nodes[i]->ScriptIndex == n->ScriptIndex) {
            switch (Nodes[i]->Type)
            {
            case NodeType::SPRITE:
            ((Sprite*)Nodes[i])->DeleteTexture();
                break;
            case NodeType::PHYSICSBODY:
            ((PhysicsBody*)Nodes[i])->DeletePhysicsBody();
                break;
            }
            ScriptableNodes.erase(ScriptableNodes.begin() + Nodes[i]->ScriptIndex);
            Nodes.erase(Nodes.begin() + i);
            break;
        }
    }
}

void Scene::UpdateChilds() {
    for (Node* n : Nodes) {
        n->UpdateChild();
    }
}

void Scene::Start(){
    AddNodesToScene("../Assets/FlappyBird/FlappyBird.vscene");
    //AddNodesToScene("../Assets/test.vscene");

    UpdateChilds();

    AssignRoot(this);
    BindNodes();
    BindFunctions();
    RunEngineFile();
    LoadNodeScripts(ScriptableNodes);
    SetNodes(ScriptableNodes);
    StartFunction(ScriptableNodes);
}

void Scene::Update(double dt){    
    UpdateChilds();
    for (Node* n : Nodes) {
        switch (n->Type)
        {
        case NodeType::PHYSICSBODY:
            ((PhysicsBody*)n)->UpdatePhysicsNode();
            break;
        }
    }
    UpdateFunction(ScriptableNodes, dt);
}

void Scene::Draw(){
    for (Node* n : Nodes) {
        switch (n->Type)
        {
        case NodeType::SPRITE:
            ((Sprite*)n)->DrawImage();
            break;
        }
    }
}

void Scene::Clean(){
    for (Node* n : Nodes) {
        switch (n->Type)
        {
        case NodeType::SPRITE:
        ((Sprite*)n)->DeleteTexture();
            break;
        case NodeType::PHYSICSBODY:
        ((PhysicsBody*)n)->DeletePhysicsBody();
            break;
        }
    }
    Nodes.clear();
    ScriptableNodes.clear();
}