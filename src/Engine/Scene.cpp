#include "Scene.h"
#include <string>

template <typename T>
void Print(T log)
{
    std::cout << log << "\n";
}

enum NodeType {
    Null,
    NODE
};

NodeType GetNodeType (std::string const& inString) {
    if (inString == "NODE") return NODE;
    return Null;
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
    s.SetRenderer(Renderer);
    s.SetCam(&cam);
    s.LoadImage("../Assets/Test1.png");

    b.ColliderSize = Vector2(100, 100);
    b.InitBoxCollider();

    p.Transform.Position = Vector2(250, 400);
    p.Transform.Angle = 44;
    p.InitPhysicsBody(PhysicsWorld, &b, b2_dynamicBody);

    p.AddChild(&b);
    p.AddChild(&s);

    Grounds.SetRenderer(Renderer);
    Grounds.SetCam(&cam);
    Grounds.LoadImage("../Assets/Test1.png");
    Grounds.Transform.Size.x = 3;

    Groundb.ColliderSize = Vector2(300, 100);
    Groundb.InitBoxCollider();

    Groundp.Transform.Position = Vector2(550, 700);
    Groundp.InitPhysicsBody(PhysicsWorld, &Groundb, b2BodyType::b2_staticBody);

    Groundp.AddChild(&Groundb);
    Groundp.AddChild(&Grounds);

    Groundss.SetRenderer(Renderer);
    Groundss.SetCam(&cam);
    Groundss.LoadImage("../Assets/Test1.png");
    Groundss.Transform.Size.x = 3;

    Groundbb.ColliderSize = Vector2(300, 100);
    Groundbb.InitBoxCollider();

    Groundpp.Transform.Position = Vector2(250, 600);
    Groundpp.InitPhysicsBody(PhysicsWorld, &Groundbb, b2BodyType::b2_staticBody);

    Groundpp.AddChild(&Groundbb);
    Groundpp.AddChild(&Groundss);

    Cirs.SetRenderer(Renderer);
    Cirs.SetCam(&cam);
    Cirs.LoadImage("../Assets/Test2.png");

    Cirb.Radius = 50;
    Cirb.InitCircleCollider();
    Cirp.Transform.Position = Vector2(250, 250);
    Cirp.InitPhysicsBody(PhysicsWorld, &Cirb, b2BodyType::b2_dynamicBody);

    Cirp.AddChild(&Cirs);
    Cirp.AddChild(&Cirb);

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

    std::string Text;

    std::ifstream SceneFile("../Assets/test.vscene"); 

    while (std::getline(SceneFile, Text)) {

        switch (GetNodeType(Text.substr((Text.find("[") + 10), (Text.find("]") - (Text.find("[") + 10)))))
        {
        case NodeType::Null:
            break;
        case NodeType::NODE:
            Nodes.push_back(new Node);
            std::string pos = Text.substr(Text.find("[POSITION(") + 10, Text.find("]", Text.find("[POSITION(")) - (Text.find("[POSITION(") + 11));
            float PosX = std::stof(pos.substr(0, pos.find(",")));
            float PosY = std::stof(pos.substr(pos.find(",") + 1, pos.length() - pos.find(",")));
            Nodes[Nodes.size() - 1]->Transform.Position.Set(PosX, PosY);
            break;
        }
    }

    SceneFile.close();
}

void Scene::Update(double dt)
{
    p.UpdatePhysicsNode();
    Groundp.UpdatePhysicsNode();
    Groundpp.UpdatePhysicsNode();
    Cirp.UpdatePhysicsNode();
    Polyp.UpdatePhysicsNode();

    p.UpdateChild();
    Groundp.UpdateChild();
    Groundpp.UpdateChild();
    Cirp.UpdateChild();
    Polyp.UpdateChild();
}

void Scene::Draw()
{
    Grounds.DrawImage();
    Groundss.DrawImage();
    s.DrawImage();
    Cirs.DrawImage();
    Polys.DrawImage();
}

void Scene::Clean()
{
    Grounds.CleanImage();
    Groundss.CleanImage();
    s.CleanImage();
    Cirs.CleanImage();
    Polys.CleanImage();

    for (int i = 0; i < (int)Nodes.size(); i++)
    {
        delete Nodes[i];
        Nodes[i] = nullptr;
    }
    
}