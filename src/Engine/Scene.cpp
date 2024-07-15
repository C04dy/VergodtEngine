#include "Scene.h"
#include <fstream>
#include <sstream>
#include "Util/SceneFileFunctions.h"
#include "squall/squall_vmstd.hpp"
#include "squall/squall_klass.hpp"

void Scene::Start(){
    std::string Line;
    std::ifstream SceneFile("Assets/test.vscene");

    std::vector<Node*> ALLNODES;

    while (std::getline(SceneFile, Line)){
        std::string CurNodeType = GetLineBetween(Line, "[NODETYPE=", "]");
        
        if(CurNodeType == "NODE"){
            Node* n = new Node;

            SetNode(n, Line);

            SetChild(n, ALLNODES, Line);

            Nodes.push_back(n);
            ALLNODES.push_back(Nodes[Nodes.size() - 1]);
        }else if(CurNodeType == "CAMERA"){
            SetNode(&Cam, Line);
        }else if(CurNodeType == "SPRITE"){
            std::string AssetFilePath = GetLineBetween(Line, "[ASSET=", "]");

            Sprite* s = new Sprite(AssetFilePath, &Cam, Renderer);

            SetNode(s, Line);

            SetChild(s, ALLNODES, Line);

            Sprites.push_back(s);
            ALLNODES.push_back(Sprites[Sprites.size() - 1]);
        }else if(CurNodeType == "PHYSICSBODY"){        
            PhysicsBody* p = new PhysicsBody;

            SetNode(p, Line);

            SetChild(p, ALLNODES, Line);

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
    
    squall::VMStd vm;
    
    squall::Klass<Vector2>(vm, "Vector2")
        .var("x", &Vector2::x)
        .var("y", &Vector2::y);

    squall::Klass<Node>(vm, "Node")
        .var("Name", &Node::Name)
        .var("Position", &Node::Position);

    std::fstream stream("../Assets/VergodtEngine.nut");

    std::string line;
    std::string s;
    bool ChangeLine = false;
    while (getline(stream, line)) {
        if (!ChangeLine) {
            s += line + "\n";
        } else {
            s += "Ns.push(test); \n";
            ChangeLine = false;
        }
        if (line.find("function SetNodes() {") != std::string::npos) {
            ChangeLine = true;
        }
    }
    stream.close();

    //vm.dofile("../Assets/VergodtEngine.nut");
    vm.dostring(s.c_str());
    vm.dofile("../Assets/test.nut");

    vm.call<void>("SetNodes");
    vm.call<void>("SetNodeVal", (Node*)Sprites[0]);
    vm.call<void>("StartFunc");
    vm.call<void>("GetNodeVal", (Node*)Sprites[0], &Sprites[0]->Position);

    std::cout << Sprites[0]->Position.x;
}

void Scene::Update(double dt){
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
        Sprites[i]->DeleteSprite();
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