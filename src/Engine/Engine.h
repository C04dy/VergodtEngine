#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include "BaseScene.h"

class b2World;

class Engine
{
public:
	void EngineStart(BaseScene* S);

	bool IsRunning() { return m_running; }
	void SetRunning(bool running) { m_running = running; }

private:
	bool m_running = true;

	b2World* m_physicsworld = nullptr;
};

