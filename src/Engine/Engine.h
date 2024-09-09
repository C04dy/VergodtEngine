#pragma once
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <string>

class b2World;

class Engine
{
public:
	void EngineStart();

	bool IsRunning() { return m_Running; }

	void SetRunning(bool Running) { m_Running = Running; }
private:
	bool m_Running = true;

	b2World* m_PhysicsWorld = nullptr;
};

