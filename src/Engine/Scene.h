#pragma once
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "Input.h"
#include "Project.h"

#include "Objects/Camera.h"
#include "Objects/Sprite.h"
#include "Objects/PhysicsBody.h"
#include "Objects/Collider.h"
#include "Objects/Node.h"

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

	void SetRenderer(SDL_Renderer* Renderer) { m_Renderer = Renderer; }
	void SetPhysicsWorldID(b2WorldId& PhysicsWorld) { m_PhysicsWorldID = PhysicsWorld; }
	void SetProject(Project* GameProject) { m_Project = GameProject; }
	void SetInput(InputManager* Input) { m_Input = Input; }

	InputManager* GetInput() { return m_Input; }
private:
	SDL_Renderer* m_Renderer = nullptr;

	b2WorldId m_PhysicsWorldID;

	InputManager* m_Input = nullptr;

	Camera* m_MainCamera;

	Project* m_Project;

	std::vector<Node*> m_Nodes;
};