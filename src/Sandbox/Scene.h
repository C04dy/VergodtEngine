#ifndef SCENE_H
#define SCENE_H

#pragma once

#include <iostream>

#include "Engine/BaseScene.h"

#include "Objects/Camera.h"
#include "Objects/Sprite.h"
#include "Objects/PhysicsBody.h"
#include "Objects/BoxCollider.h"
#include "Objects/CircleCollider.h"

class Scene : public BaseScene
{
public:
	Scene();
	~Scene();

    void Start();
	void Update(double dt);
	void Draw();
	void Clean();

private:
	Camera cam;
	
	Sprite s;
	PhysicsBody p;
	BoxCollider b;

	Sprite Grounds;
	PhysicsBody Groundp;
	BoxCollider Groundb;

	Sprite Cirs;
	PhysicsBody Cirp;
	CircleCollider Cirb;

	Sprite Groundss;
	PhysicsBody Groundpp;
	BoxCollider Groundbb;

	b2CircleShape shape;
	b2BodyDef bd;
	b2Body* m_bodies;
};

#endif