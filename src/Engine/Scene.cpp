#include "Scene.h"
#include <fstream>
#include <algorithm> 
#include <ctime>
#include "Util/SceneFileFunctions.h"

void Scene::AddNodesToScene(const std::string& SceneFilePath) {
    std::string Line;
    std::ifstream SceneFile(SceneFilePath);

    if (SceneFile.fail()) {
        std::cout << "Scene File did not found.\n";
        return;
    }

    int IndexOffset = Nodes.size();

    while (std::getline(SceneFile, Line)) {
        if (Line.at(0) != '#') {
            std::string CurNodeType = GetLineBetween(Line, "[NODETYPE=", "]");
            if(CurNodeType == "NODE"){
                Node* n = new Node();

                SetNode(n, Line, Input);
 
                SetChild(n, Nodes, Line, IndexOffset);

                Nodes.push_back(n);
            }else if(CurNodeType == "CAMERA"){
                SetNode(&Cam, Line, Input);

                Nodes.push_back(&Cam);
            }else if(CurNodeType == "SPRITE"){
                std::string AssetFilePath = GetLineBetween(Line, "[ASSET=", "]");

                Sprite* s = new Sprite();

                s->InitSprite(AssetFilePath, &Cam, Renderer);

                SetNode(s, Line, Input);

                SetChild(s, Nodes, Line, IndexOffset);

                Nodes.push_back(s);
            }else if(CurNodeType == "PHYSICSBODY"){        
                PhysicsBody* p = new PhysicsBody;

                SetNode(p, Line, Input);

                SetChild(p, Nodes, Line, IndexOffset);

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
        }
    }
    SceneFile.close();
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

    for (Node* n : Nodes) {
        if (n->Script != nullptr) {
            n->Script->StartScript();
        }
    }
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
        if (n->Script != nullptr) {
            n->Script->UpdateScript();
        }
    }
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
}