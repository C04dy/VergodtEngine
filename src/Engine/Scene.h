#pragma once
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "Input.h"

#include "Objects/Camera.h"
#include "Objects/Sprite.h"
#include "Objects/PhysicsBody.h"
#include "Objects/Node.h"

class b2World;

class Scene
{
public:
	void Start();
	void Update(double Delta);
	void Draw();
	void Clean();

	void UpdateChilds();

	void AddNodesToScene(const std::string& SceneFilePath);
	void RemoveNodeFromScene(Node* _Node);

	void SetRenderer(SDL_Renderer* Renderer) { Renderer = Renderer; }
	void SetPhysicsWorld(b2World* PhysicsWorld) { PhysicsWorld = PhysicsWorld; }

	std::string GetGameName() { return m_GameName; }
	void SetGameName(const std::string& GameName) { m_GameName = GameName; }

	int GetWindowWidth() { return m_WindowWidth; }
	int GetWindowHeight() { return m_WindowHeight; }

	void SetWindowsWidth(int Width) { m_WindowWidth = Width; }
	void SetWindowsHeight(int Height) { m_WindowHeight = Height; }

	void SetInput(InputManager* Input) { m_Input = Input; }
	InputManager* GetInput() { return m_Input; }
private:
	// these will be moved somewhere else
	std::string m_GameName = "VergodtEngineGame";

	int m_WindowWidth = 1280, m_WindowHeight = 720;
private:
	SDL_Renderer* m_Renderer;

	b2World* m_PhysicsWorld;

	InputManager* m_Input;

	Camera m_MainCamera;

	std::vector<Node*> m_Nodes;
};