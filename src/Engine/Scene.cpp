#include "Scene.h"
#include <string>

template <typename T>
void Print(T log)
{
    std::cout << log << "\n";
}

enum NodeType {
    Null,
    NODE,
    SPRITE,
    CAM,
    PHYSICSBODY
};

NodeType GetNodeType (std::string const& inString) 
{
    if (inString == "NODE") return NODE;
    if (inString == "SPRITE") return SPRITE;
    if (inString == "CAMERA") return CAM;
    if (inString == "PHYSICSBODY") return PHYSICSBODY;
    return Null;
}


std::string GetLineBetween(std::string Text, std::string Start, std::string Finish){
    return Text.substr( Text.find( Start ) + ( Start.length()  ) , Text.find( Finish, Text.find(Start) ) - ( Text.find(Start) + (Start.length()  ) ) );
}

void SetNode(Node* node, std::string Line, std::vector<Node*> Child)
{
    std::string pos = GetLineBetween(Line, "[POSITION(", ")]");
    

    float PosX = std::stof(pos.substr(0, pos.find(",")).c_str());
    float PosY = std::stof(pos.substr(pos.find(",") + 1, pos.length() - pos.find(",")).c_str());
    node->Transform.Position.Set(PosX, PosY);

    node->Name = GetLineBetween(Line, "[NAME=", "]");

    std::string size = Line.substr(Line.find("[SIZE(") + 6, Line.find("]", Line.find("[SIZE(")) - (Line.find("[SIZE(") + 7));
    float sizeX = std::stof(size.substr(0, size.find(",")).c_str());
    float sizeY = std::stof(size.substr(size.find(",") + 1, size.length() - size.find(",")).c_str());
    node->Transform.Size.Set(sizeX, sizeY);

    std::string ang = Line.substr(Line.find("[ANGLE(") + 7, Line.find("]", Line.find("[ANGLE(")) - (Line.find("[ANGLE(") + 8));
    float angle = std::stof(ang);
    node->Transform.Angle = angle;

    std::string Havechild = GetLineBetween(Line, "[CHILD=", "]");

    if(Havechild == "TRUE"){
        for (int i = 0; i < (int)Child.size(); i++)
        {
            if(Child[i]->Name == GetLineBetween(Line, "[CHILDNAME=", "]")){
                node->AddChild(Child[i]);
            }
        }
    }
}

Scene::Scene()
{
    SetWindowsWidth(1280);
    SetWindowsHeight(720);
    SetGameName("VergodtEngineGameHello");
}

Scene::~Scene()
{

}

void Scene::Start()
{
    std::vector<Node*> Childs;

    std::string Line;

    std::ifstream SceneFile("../Assets/test.vscene"); 

    while (std::getline(SceneFile, Line)) {
        switch (GetNodeType(Line.substr((Line.find("[") + 10), (Line.find("]") - (Line.find("[") + 10)))))
        {
        case NodeType::Null:
            Print("Cannot load the node");
            break;
        case NodeType::NODE:
            Nodes.push_back(new Node);
            SetNode(Nodes[Nodes.size() - 1], Line, Childs);
            Childs.push_back(Nodes[Nodes.size() - 1]);
            break;
        case NodeType::SPRITE:{
            Sprites.push_back(new Sprite);
            SetNode(Sprites[Sprites.size() - 1], Line, Childs);     
            Sprites[Sprites.size() - 1]->SetCam(&Cam);
            Sprites[Sprites.size() - 1]->SetRenderer(Renderer);
            std::string sprite = GetLineBetween(Line, "[ASSET=", "]");
            Sprites[Sprites.size() - 1]->LoadImage(sprite);
            Childs.push_back(Sprites[Sprites.size() - 1]);
            }break;
        case NodeType::CAM:
            SetNode(&Cam, Line, Childs);
            break;
        case NodeType::PHYSICSBODY:
            PhysicsBodys.push_back(new PhysicsBody);
            SetNode(PhysicsBodys[PhysicsBodys.size() - 1], Line, Childs);
            std::string ColType = GetLineBetween(Line, "[COLLIDER=", "]");
            if(ColType == "BOX"){
                std::string ColSize = GetLineBetween(Line, "[COLLIDERSIZE(", ")]");

                float ColSizeX = std::stof(ColSize.substr(0, ColSize.find(",")));
                float ColSizeY = std::stof(ColSize.substr(ColSize.find(",") + 1, ColSize.length() - ColSize.find(",")).c_str());

                std::string BodyType = GetLineBetween(Line, "[PHYSICSTYPE=", "]");

                if(BodyType == "KINEMATIC") {
                    PhysicsBodys[PhysicsBodys.size() - 1]->InitPhysicsBodyBox(PhysicsWorld, b2BodyType::b2_kinematicBody, ColSizeX, ColSizeY);
                }else if(BodyType == "DYNAMIC") {
                    PhysicsBodys[PhysicsBodys.size() - 1]->InitPhysicsBodyBox(PhysicsWorld, b2BodyType::b2_dynamicBody, ColSizeX, ColSizeY);
                }else{
                    PhysicsBodys[PhysicsBodys.size() - 1]->InitPhysicsBodyBox(PhysicsWorld, b2BodyType::b2_staticBody, ColSizeX, ColSizeY); 
                }
            } else if (ColType == "CIRCLE"){
                
                float radius = std::stof(GetLineBetween(Line, "[RADIUS=", "]").c_str());

                std::string BodyType = GetLineBetween(Line, "[PHYSICSTYPE=", "]");

                if(BodyType == "KINEMATIC") {
                    PhysicsBodys[PhysicsBodys.size() - 1]->InitPhysicsBodyCircle(PhysicsWorld, b2BodyType::b2_kinematicBody, radius);
                }else if(BodyType == "DYNAMIC") {
                    PhysicsBodys[PhysicsBodys.size() - 1]->InitPhysicsBodyCircle(PhysicsWorld, b2BodyType::b2_dynamicBody, radius);
                }else{
                    PhysicsBodys[PhysicsBodys.size() - 1]->InitPhysicsBodyCircle(PhysicsWorld, b2BodyType::b2_staticBody, radius); 
                }
            } else if (ColType == "POLYGON"){
                
                std::string Polygons = GetLineBetween(Line, "[POLYGONS=", "]");
                Vector2 Polys[std::stoi(Polygons.substr(0, Polygons.length() - Polygons.find("=")))];

                std::string PolyPos = Polygons.substr(Polygons.find("("), Polygons.size() - Polygons.find("("));

                for (int i = 0; i < (int)(sizeof(Polys) / sizeof(Vector2)); i++)
                {
                    std::string CurPolyPos = GetLineBetween(PolyPos, "(", ")");
                    
                    float PolyPosX = std::stof(CurPolyPos.substr(0, CurPolyPos.find(",")).c_str());
                    float PolyPosY = std::stof(CurPolyPos.substr(CurPolyPos.find(",") + 1, CurPolyPos.size() - CurPolyPos.find(",")).c_str());
                    

                    Polys[i].Set(PolyPosX, PolyPosY);

                    PolyPos = PolyPos.substr(PolyPos.find(")") + 1, PolyPos.size() - PolyPos.find(")"));
                }
                
                std::string BodyType = GetLineBetween(Line, "[PHYSICSTYPE=", "]");

                if(BodyType == "KINEMATIC") {
                    PhysicsBodys[PhysicsBodys.size() - 1]->InitPhysicsBodyPolygon(PhysicsWorld, b2BodyType::b2_kinematicBody, Polys, sizeof(Polys) / sizeof(Vector2));
                }else if(BodyType == "DYNAMIC") {
                    PhysicsBodys[PhysicsBodys.size() - 1]->InitPhysicsBodyPolygon(PhysicsWorld, b2BodyType::b2_dynamicBody, Polys, sizeof(Polys) / sizeof(Vector2));
                }else{
                    PhysicsBodys[PhysicsBodys.size() - 1]->InitPhysicsBodyPolygon(PhysicsWorld, b2BodyType::b2_staticBody, Polys, sizeof(Polys) / sizeof(Vector2)); 
                }
            }
            Childs.push_back(PhysicsBodys[PhysicsBodys.size() - 1]);
            break;
        }
    }

    for (int i = 0; i < (int)Childs.size(); i++)
    {
        Childs[i] = nullptr;
    }

    SceneFile.close();
}

void Scene::Update(double dt)
{
    for (int i = 0; i < (int)PhysicsBodys.size(); i++)
    {
        PhysicsBodys[i]->UpdatePhysicsNode();
        PhysicsBodys[i]->UpdateChild();
    }
    for (int i = 0; i < (int)Sprites.size(); i++)
    {
        Sprites[i]->UpdateChild();
    }
    for (int i = 0; i < (int)Nodes.size(); i++)
    {
        Nodes[i]->UpdateChild();
    }
}

void Scene::Draw()
{
    for (int i = 0; i < (int)Sprites.size(); i++)
    {
        Sprites[i]->DrawImage();
    }
}

void Scene::Clean()
{
    for (int i = 0; i < (int)Sprites.size(); i++)
    {
        Sprites[i]->CleanImage();
        delete Sprites[i];
        Sprites[i] = nullptr;
    }
    for (int i = 0; i < (int)Nodes.size(); i++)
    {
        delete Nodes[i];
        Nodes[i] = nullptr;
    }
    for (int i = 0; i < (int)PhysicsBodys.size(); i++)
    {
        delete PhysicsBodys[i];
        PhysicsBodys[i] = nullptr;
    }
}