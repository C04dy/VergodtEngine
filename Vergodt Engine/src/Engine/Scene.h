#pragma once
#include <iostream>
#include <SDL2/SDL.h>

#include "Objects/Camera.h"
#include "Objects/Sprite.h"
#include "Objects/PhysicsBody.h"
#include "Objects/BoxCollider.h"

class Scene
{
public:
	void Start(SDL_Renderer* r);
	void Update(double dt);
	void PhysicsUpdate(double dt);
	void Draw();
	void Clean();

	std::string GetGameName() { return m_gamename; }
	void SetGameName(std::string gamename) { m_gamename = gamename; }

	int GetWindowWidth() { return m_windowwidth; }
	int GetWindowHeight() { return m_windowheight; }

	void SetWindowsWidth(int w) { m_windowwidth = w; }
	void SetWindowsHeight(int h) { m_windowheight = h; }

private:
	std::string m_gamename = "VergodtEngineGame";

	int m_windowwidth = 1280, m_windowheight = 720;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	float timer;

	Sprite s;
	Camera cam;

	BoxCollider b;
	PhysicsBody p;
};

