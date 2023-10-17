#ifndef SCENE_H
#define SCENE_H

#pragma once

#include "iostream"

#include "Engine/BaseScene.h"

#include "Objects/Camera.h"
#include "Objects/Sprite.h"
#include "Objects/PhysicsBody.h"
#include "Objects/BoxCollider.h"

class Scene : public BaseScene
{
public:
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

};

#endif