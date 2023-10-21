#include "Scene.h"
#include <string>

template <typename T> void Logs(T log)
{
    std::cout << log << "\n";
}


void Scene::Start()
{
    this->SetWindowsHeight(1280);
    this->SetWindowsWidth(720);

    s.SetRenderer(Renderer);
    s.SetCam(&cam);
    s.LoadImage("Assets/Test1.png");
    
    b.ColliderSize = Vector2(100, 100);
    b.InitBoxCollider();

    p.Transform.Position = Vector2(250, 400);
    p.Transform.Angle = 24;
    p.InitPhysicsBody(PhysicsWorld, &b, b2_dynamicBody);

    p.AddChild(&b);
    p.AddChild(&s);

    Grounds.SetRenderer(Renderer);
    Grounds.SetCam(&cam);
    Grounds.LoadImage("Assets/Test1.png");
    Grounds.Transform.Size.x = 3;

    Groundb.ColliderSize = Vector2(300, 100);
    Groundb.InitBoxCollider();

    Groundp.Transform.Position = Vector2(250, 700);
    Groundp.InitPhysicsBody(PhysicsWorld, &Groundb, b2BodyType::b2_staticBody);

    Groundp.AddChild(&Groundb);
    Groundp.AddChild(&Grounds);

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
    
    p.UpdateChild();
    Groundp.UpdateChild();

    Cirp.UpdatePhysicsNode();
    Cirp.UpdateChild();
}

void Scene::Draw()
{
    Grounds.DrawImage();
    s.DrawImage();
    Cirs.DrawImage();
}

void Scene::Clean()
{
    Grounds.CleanImage();
    s.CleanImage();
    Cirs.CleanImage();
}