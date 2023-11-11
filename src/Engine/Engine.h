#pragma once
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <iostream>
#include <string>


class b2World;

class Engine
{
public:
	void EngineStart();

	bool IsRunning() { return m_running; }
	void SetRunning(bool running) { m_running = running; }

private:
	bool m_running = true;

	b2World* m_physicsworld = nullptr;
};

