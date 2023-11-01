#pragma once
#include <iostream>
#include <SDL2/SDL.h>

#include "Objects/Camera.h"
#include "Objects/Sprite.h"
#include "Objects/PhysicsBody.h"
#include "Objects/BoxCollider.h"
#include "Objects/CircleCollider.h"

class b2World;

class Scene
{
public:
	Scene();
    ~Scene();

	virtual void Start();
	virtual void Update(double dt);
	virtual void Draw();
	virtual void Clean();

	void SetRenderer(SDL_Renderer* r) { Renderer = r; }
	void SetPhysicsWorld(b2World* w) { PhysicsWorld = w; }

	std::string GetGameName() { return m_gamename; }
	void SetGameName(std::string gamename) { m_gamename = gamename; }

	int GetWindowWidth() { return m_windowwidth; }
	int GetWindowHeight() { return m_windowheight; }

	void SetWindowsWidth(int w) { m_windowwidth = w; }
	void SetWindowsHeight(int h) { m_windowheight = h; }

protected:
	SDL_Renderer* Renderer;

	b2World* PhysicsWorld;
private:
	std::string m_gamename = "VergodtEngineGame";

	int m_windowwidth = 1280, m_windowheight = 720;

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