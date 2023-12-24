#include "Scene.h"
#include <fstream>
#include "Util/SceneFileFunctions.h"

template <typename T>
void Log(T log)
{
    std::cout << log << "\n";
}

void SetNode(Node* n, const std::string& Line){
    std::string pose = GetLineBetween(Line, "[POSITION(", ")]");
    std::string size = GetLineBetween(Line, "[SIZE(", ")]");
    std::string angle = GetLineBetween(Line, "[ANGLE(", ")]");
    
    n->Transform.Position = Vector2(std::stof(GetLineBetween(pose, 0, ",")), std::stof(GetLineBetween(pose, ",")));
    n->Transform.Size = Vector2(std::stof(GetLineBetween(size, 0, ",")), std::stof(GetLineBetween(size, ",")));
    n->Transform.Angle = std::stof(angle);
    n->Name = GetLineBetween(Line, "[NAME=", "]");
}

void SetChild(Node* n, std::vector<Node*> AN, const std::string& Line){
    int ChildSize = std::stoi(GetLineBetween(Line, "[CHILD=", "]"));
    for (int i = 0; i < ChildSize; i++)
    {
        n->AddChild(AN[(AN.size() - 1) - i]);
    }
    
}

enum class ScriptState{
    START,
    UPDATE
};

void SetScript(Node* n, const std::string& Line){
    if(GetLineBetween(Line, "[SCRIPT=", "]") != "NULL"){
        std::string ln;
        std::ifstream ScriptFile(GetLineBetween(Line, "[SCRIPT=", "]"));

        StartNode* s = new StartNode;
        UpdateNode* u = new UpdateNode;
        n->Script = new VisualScript;

        n->Script->InitVisualScript(s, u);

        ScriptState State = ScriptState::START;

        while (std::getline(ScriptFile, ln)){
            if(GetLineBetween(ln, "[", "]") == "START"){
                State = ScriptState::START;
            }else if(GetLineBetween(ln, "[", "]") == "UPDATE"){
                State = ScriptState::UPDATE;
            }else if(GetLineBetween(ln, "[", "]") == "PRINT"){
                PrintNode* p = new PrintNode;

                p->Message = GetLineBetween(ln, "(", ")");

                if(State == ScriptState::START){
                    s->ConnectedNodes.push_back(p);
                }
            }
        }

        ScriptFile.close();
    }
}

void Scene::Start(){
    std::string Line;
    std::ifstream SceneFile("../Assets/test.vscene");

    std::vector<Node*> ALLNODES;

    while (std::getline(SceneFile, Line)){
        std::string CurNodeType = GetLineBetween(Line, "[NODETYPE=", "]");
        if(CurNodeType == "NODE"){
            Node* n = new Node;

            SetNode(n, Line);

            SetChild(n, ALLNODES, Line);

            SetScript(n, Line);

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


    for (int i = 0; i < (int)ALLNODES.size(); i++)
    {
        if(ALLNODES[i]->Script != nullptr){
            ALLNODES[i]->Script->StartScript();
        }
    }
    
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