#include "Scene.h"
#include "Scripting/Squirrel/SquirrelBindings.hpp"
#include <fstream>
#include <ctime>

void Scene::Start(){
    std::string Line;
    std::ifstream SceneFile("../Assets/FlappyBird.vscene");

    std::vector<Node*> ALLNODES;

    while (std::getline(SceneFile, Line)){
        std::string CurNodeType = GetLineBetween(Line, "[NODETYPE=", "]");
        
        if(CurNodeType == "NODE"){
            Node* n = new Node("");

            SetNode(n, Line);

            SetChild(n, ALLNODES, Line);
 
            if (SetNodesScript(Line, n) == true) {
                ScriptableNodes.push_back(n);
            }
    
            Nodes.push_back(n);
            ALLNODES.push_back(Nodes[Nodes.size() - 1]);
        }else if(CurNodeType == "CAMERA"){
            SetNode(&Cam, Line);

            if (SetNodesScript(Line, (Node*)&Cam) == true) {
                ScriptableNodes.push_back(&Cam);
            }
        }else if(CurNodeType == "SPRITE"){
            std::string AssetFilePath = GetLineBetween(Line, "[ASSET=", "]");

            Sprite* s = new Sprite(AssetFilePath, &Cam, Renderer);

            SetNode(s, Line);

            SetChild(s, ALLNODES, Line);

            if (SetNodesScript(Line, (Node*)s) == true) {
                ScriptableNodes.push_back(s);
            }

            Sprites.push_back(s);
            ALLNODES.push_back(Sprites[Sprites.size() - 1]);
        }else if(CurNodeType == "PHYSICSBODY"){        
            PhysicsBody* p = new PhysicsBody;

            SetNode(p, Line);

            SetChild(p, ALLNODES, Line);

            if (SetNodesScript(Line, (Node*)p) == true) {
                ScriptableNodes.push_back(p);
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

            PhysicsBodys.push_back(p); 
            ALLNODES.push_back(PhysicsBodys[PhysicsBodys.size() - 1]);        
        }
    }
    SceneFile.close();

    BindNodes(SquirrelVirtualMachine);
    BindFunctions(SquirrelVirtualMachine, Input);
    EditEngineFile(SquirrelVirtualMachine, ScriptableNodes);
    LoadNodeScripts(SquirrelVirtualMachine, ScriptableNodes);
    SetNodes(SquirrelVirtualMachine);
    StartFunction(SquirrelVirtualMachine, ScriptableNodes);
}

void Scene::Update(double dt){    
    UpdateFunction(SquirrelVirtualMachine, ScriptableNodes, dt);
    
    for(int i = 0; i < (int)PhysicsBodys.size(); i++){
        PhysicsBodys[i]->UpdatePhysicsNode();
        PhysicsBodys[i]->UpdateChild();
    }
    for(int i = 0; i < (int)Sprites.size(); i++){
        Sprites[i]->UpdateChild();
    }
    for(int i = 0; i < (int)Nodes.size(); i++){
        Nodes[i]->UpdateChild();
    }
}

void Scene::Draw(){
    for(int i = 0; i < (int)Sprites.size(); i++){
        Sprites[i]->DrawImage();
    }
}

void Scene::Clean(){
    for(int i = 0; i < (int)Sprites.size(); i++){
        Sprites[i]->DeleteTexture();
        delete Sprites[i];
        Sprites[i] = nullptr;
    }
    for(int i = 0; i < (int)PhysicsBodys.size(); i++){
        PhysicsBodys[i]->DeletePhysicsBody();
        delete PhysicsBodys[i];
        PhysicsBodys[i] = nullptr;
    }
    for (int i = 0; i < (int)Nodes.size(); i++)
    {
        delete Nodes[i];
        Nodes[i] = nullptr;
    }
    
    Nodes.clear();
    Sprites.clear();
    PhysicsBodys.clear();
}