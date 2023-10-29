#include "Scene.h"
#include <string>

template <typename T>
void Print(T log)
{
    std::cout << log << "\n";
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
    s.LoadImage("Assets/Test1.png");
    
    b.ColliderSize = Vector2(100, 100);
    b.InitBoxCollider();

    p.Transform.Position = Vector2(250, 400);
    p.Transform.Angle = 44;
    p.InitPhysicsBody(PhysicsWorld, &b, b2_dynamicBody);

    p.AddChild(&b);
    p.AddChild(&s);

    Grounds.SetRenderer(Renderer);
    Grounds.SetCam(&cam);
    Grounds.LoadImage("Assets/Test1.png");
    Grounds.Transform.Size.x = 3;

    Groundb.ColliderSize = Vector2(300, 100);
    Groundb.InitBoxCollider();

    Groundp.Transform.Position = Vector2(550, 700);
    Groundp.InitPhysicsBody(PhysicsWorld, &Groundb, b2BodyType::b2_staticBody);

    Groundp.AddChild(&Groundb);
    Groundp.AddChild(&Grounds);

    Groundss.SetRenderer(Renderer);
    Groundss.SetCam(&cam);
    Groundss.LoadImage("Assets/Test1.png");
    Groundss.Transform.Size.x = 3;

    Groundbb.ColliderSize = Vector2(300, 100);
    Groundbb.InitBoxCollider();

    Groundpp.Transform.Position = Vector2(250, 600);
    Groundpp.InitPhysicsBody(PhysicsWorld, &Groundbb, b2BodyType::b2_staticBody);

    Groundpp.AddChild(&Groundbb);
    Groundpp.AddChild(&Groundss);

    Cirs.SetRenderer(Renderer);
    Cirs.SetCam(&cam);
    Cirs.LoadImage("Assets/Test2.png");

    Cirb.Radius = 50;
    Cirb.InitCircleCollider();
    Cirp.Transform.Position = Vector2(250, 250);
    Cirp.InitPhysicsBody(PhysicsWorld, &Cirb, b2BodyType::b2_dynamicBody);


    Cirp.AddChild(&Cirs);
    Cirp.AddChild(&Cirb);
}

void Scene::Update(double dt)
{
    p.UpdatePhysicsNode();
    Groundp.UpdatePhysicsNode();
    Groundpp.UpdatePhysicsNode();
    Cirp.UpdatePhysicsNode();
    
    p.UpdateChild();
    Groundp.UpdateChild();
    Groundpp.UpdateChild();  
    Cirp.UpdateChild();
}

void Scene::Draw()
{
    Grounds.DrawImage();
    Groundss.DrawImage();
    s.DrawImage();
    Cirs.DrawImage();
}

void Scene::Clean()
{
    Grounds.CleanImage();
    Groundss.CleanImage();
    s.CleanImage();
    Cirs.CleanImage();
}