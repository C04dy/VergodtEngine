#pragma once
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "Input.h"

#include "Objects/Camera.h"
#include "Objects/Sprite.h"
#include "Objects/PhysicsBody.h"
#include "Objects/Node.h"

#include "simplesquirrel/simplesquirrel.hpp"

class b2World;

class Scene
{
public:
	void Start();
	void Update(double dt);
	void Draw();
	void Clean();

	std::vector<Node*> AddNodesToScene(const std::string& SceneFilePath);
	void RemoveNodeFromScene(const std::string& NodeName);

	void SetRenderer(SDL_Renderer* _Renderer) { Renderer = _Renderer; }
	void SetPhysicsWorld(b2World* _PhysicsWorld) { PhysicsWorld = _PhysicsWorld; }

	std::string GetGameName() { return m_gamename; }
	void SetGameName(const std::string& GameName) { m_gamename = GameName; }

	int GetWindowWidth() { return m_windowwidth; }
	int GetWindowHeight() { return m_windowheight; }

	void SetWindowsWidth(int Width) { m_windowwidth = Width; }
	void SetWindowsHeight(int Height) { m_windowheight = Height; }

	void SetInput(InputManager* _Input) { Input = _Input; }
	InputManager* GetInput() { return Input; }

	void SetSquirrelVM(ssq::VM* vm) { SquirrelVirtualMachine = vm; }
	ssq::VM* GetSquirrelVM() { return SquirrelVirtualMachine; }
protected:
	SDL_Renderer* Renderer;

	b2World* PhysicsWorld;

	InputManager* Input;

	ssq::VM* SquirrelVirtualMachine;
private:
	std::string m_gamename = "VergodtEngineGame";

	int m_windowwidth = 1280, m_windowheight = 720;
private:
	bool Hi = false;

	Camera Cam;

	std::vector<Node*> ScriptableNodes;
	std::vector<Node*> Nodes;
};