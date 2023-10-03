#include "Scene.h"
#include "box2d/b2_world.h"

b2Vec2 m_gravity = b2Vec2(0, -10);
b2World w(m_gravity);
void Scene::Start(SDL_Renderer* r)
{
	s.SetRenderer(r);
	s.LoadImage("Assets/Test.png");
	s.SetCam(&cam);
	s.Transform.Size.setX(0.25);
	s.Transform.Size.setY(0.25);
	
	p.Transform.Position.setX(500);
	p.Transform.Position.setY(0);

	b.ColliderSize = Vector2(10, 10);
	b.InitBoxCollider();
	
	p.InitPhysicsBody(w, b.GetFixture(), b2_dynamicBody);
	p.AddChild(&s);
	s.LocalPosition.setX(0);
	s.LocalPosition.setY(0);
}

void Scene::Update(double dt)
{
	p.UpdateChild();
}

void Scene::PhysicsUpdate(double dt)
{
	w.Step(dt, velocityIterations, positionIterations);
	p.UpdatePhysicsNode();
}

void Scene::Draw()
{
	s.DrawImage();
}

void Scene::Clean()
{
	s.CleanImage();
}