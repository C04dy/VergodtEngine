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
    CAM
};

NodeType GetNodeType (std::string const& inString) 
{
    if (inString == "NODE") return NODE;
    if (inString == "SPRITE") return SPRITE;
    if (inString == "CAMERA") return CAM;
    return Null;
}

void SetNode(Node* node, std::string Line)
{
    std::string pos = Line.substr(Line.find("[POSITION(") + 10, Line.find("]", Line.find("[POSITION(")) - (Line.find("[POSITION(") + 11));
    float PosX = std::stof(pos.substr(0, pos.find(",")));
    float PosY = std::stof(pos.substr(pos.find(",") + 1, pos.length() - pos.find(",")));
    node->Transform.Position.Set(PosX, PosY);

    std::string size = Line.substr(Line.find("[SIZE(") + 6, Line.find("]", Line.find("[SIZE(")) - (Line.find("[SIZE(") + 7));
    float sizeX = std::stof(size.substr(0, size.find(",")));
    float sizeY = std::stof(size.substr(size.find(",") + 1, size.length() - size.find(",")));
    node->Transform.Size.Set(sizeX, sizeY);

    std::string ang = Line.substr(Line.find("[ANGLE(") + 7, Line.find("]", Line.find("[ANGLE(")) - (Line.find("[ANGLE(") + 8));
    float angle = std::stof(ang);
    node->Transform.Angle = angle;

    std::string child = Line.substr(Line.find("[CHILD=") + 7, Line.find("]", Line.find("[CHILD=")) - (Line.find("[CHILD=") + 7));
    if(child == "TRUE"){
        Print("have");
    }else {
        Print("Do not have");
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
    Grounds.SetRenderer(Renderer);
    Grounds.SetCam(&cam);
    Grounds.LoadImage("../Assets/Test1.png");
    Grounds.Transform.Size.x = 3;

    Groundb.ColliderSize = Vector2(300, 100);
    Groundb.InitBoxCollider();

    Groundp.Transform.Position = Vector2(250, 650);
    Groundp.InitPhysicsBody(PhysicsWorld, &Groundb, b2BodyType::b2_staticBody);

    Groundp.AddChild(&Groundb);
    Groundp.AddChild(&Grounds);

    Polys.SetRenderer(Renderer);
    Polys.SetCam(&cam);
    Polys.LoadImage("../Assets/Test3.png");

    Vector2 pols[3];
    pols[0].Set(0.0f, -50.0f);
    pols[1].Set(50.0f, 50.0f);
    pols[2].Set(-50.0f, 50.0f);

    Polyc.InitPolygonCollider(pols, sizeof(pols)/sizeof(pols[0]));
    Polyp.Transform.Position = Vector2(250, 0);
    Polyp.InitPhysicsBody(PhysicsWorld, &Polyc, b2BodyType::b2_dynamicBody);

    Polyp.AddChild(&Polys);
    Polyp.AddChild(&Polyc);


    std::string Line;

    std::ifstream SceneFile("../Assets/test.vscene"); 

    while (std::getline(SceneFile, Line)) {

        switch (GetNodeType(Line.substr((Line.find("[") + 10), (Line.find("]") - (Line.find("[") + 10)))))
        {
        case NodeType::Null:
            break;
        case NodeType::NODE:
            Nodes.push_back(new Node);
            SetNode(Nodes[Nodes.size() - 1], Line);
            break;
        case NodeType::SPRITE:{
            Sprites.push_back(new Sprite);
            SetNode(Sprites[Sprites.size() - 1], Line);
            Sprites[Sprites.size() - 1]->SetRenderer(Renderer);
            std::string sprite = Line.substr(Line.find("[ASSET=") + 7, Line.find("]", Line.find("[ASSET=")) - (Line.find("[ASSET=") + 7));
            Sprites[Sprites.size() - 1]->LoadImage(sprite);
            }break;
        case CAM:
            Cameras.push_back(new Camera);
            SetNode(Cameras[Cameras.size() - 1], Line);
            std::string active = Line.substr(Line.find("[ACTIVE=") + 8, Line.find("]", Line.find("[ACTIVE=")) - (Line.find("[ACTIVE=") + 8));
            if(active == "TRUE"){
                Cameras[Cameras.size() - 1]->IsActive = true;
            }else{
                Cameras[Cameras.size() - 1]->IsActive = false;
            }
            break;
        }
    }

    for (int i = 0; i < (int)Cameras.size(); i++)
    {
        if(Cameras[i]->IsActive){
            for (int j = 0; j < (int)Sprites.size(); j++)
            {
                Sprites[j]->SetCam(Cameras[i]);
            }
            break;
        }
    }
    

    SceneFile.close();
}

void Scene::Update(double dt)
{
    Groundp.UpdatePhysicsNode();
    Polyp.UpdatePhysicsNode();

    Groundp.UpdateChild();
    Polyp.UpdateChild();
}

void Scene::Draw()
{
    Grounds.DrawImage();
    Polys.DrawImage();

    for (int i = 0; i < (int)Sprites.size(); i++)
    {
        Sprites[i]->DrawImage();
    }
    
}

void Scene::Clean()
{
    Grounds.CleanImage();
    Polys.CleanImage();

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
}