#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <SDL3/SDL.h>

#include "Objects/Camera.h"
#include "Objects/Sprite.h"
#include "Objects/PhysicsBody.h"
#include "Objects/Node.h"

class b2World;

class Scene
{
public:
	Scene();
	~Scene();

	void Start();
	void Update(double dt);
	void Draw();
	void Clean();

	void SetRenderer(SDL_Renderer* r) { Renderer = r; }
	void SetPhysicsWorld(b2World* w) { PhysicsWorld = w; }

	std::string GetGameName() { return m_gamename; }
	void SetGameName(std::string gamename) { m_gamename = gamename; }

	int GetWindowWidth() { return m_windowwidth; }
	int GetWindowHeight() { return m_windowheight; }

	void SetWindowsWidth(int w) { m_windowwidth = w; }
	void SetWindowsHeight(int h) { m_windowheight = h; }

	float mx, my;

protected:
	SDL_Renderer* Renderer;

	b2World* PhysicsWorld;
private:
	std::string m_gamename = "VergodtEngineGame";

	int m_windowwidth = 1280, m_windowheight = 720;

private:
	Camera Cam;

	std::vector<Node*> Nodes;
	std::vector<Sprite*> Sprites;
	std::vector<PhysicsBody*> PhysicsBodys;
};