#pragma once
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "Input.h"

#include "Objects/Camera.h"
#include "Objects/Sprite.h"
#include "Objects/PhysicsBody.h"
#include "Objects/Node.h"

#include "VisualScripting/ScriptingNodes.hpp"

class b2World;

class Scene
{
public:
	void Start();
	void Update(double dt);
	void Draw();
	void Clean();

	void SetRenderer(SDL_Renderer* _Renderer) { Renderer = _Renderer; }
	void SetPhysicsWorld(b2World* _PhysicsWorld) { PhysicsWorld = _PhysicsWorld; }

	std::string GetGameName() { return m_gamename; }
	void SetGameName(std::string GameName) { m_gamename = GameName; }

	int GetWindowWidth() { return m_windowwidth; }
	int GetWindowHeight() { return m_windowheight; }

	void SetWindowsWidth(int Width) { m_windowwidth = Width; }
	void SetWindowsHeight(int Height) { m_windowheight = Height; }

	void SetInput(Input* _Input) { Input = _Input; }
protected:
	SDL_Renderer* Renderer;

	b2World* PhysicsWorld;

	Input* Input;
private:
	std::string m_gamename = "VergodtEngineGame";

	int m_windowwidth = 1280, m_windowheight = 720;
private:
	Camera* Cam = nullptr;

	PhysicsBody* Test1p = nullptr;
	Sprite* Test1s = nullptr;

	PhysicsBody* Test2p = nullptr;
	Sprite* Test2s = nullptr;

	PhysicsBody* Test3p = nullptr;
	Sprite* Test3s = nullptr;

	PhysicsBody* Test4p = nullptr;
	Sprite* Test4s = nullptr;

	PhysicsBody* Test5p = nullptr;
	Sprite* Test5s = nullptr;

	VisualScript* script = nullptr;
};