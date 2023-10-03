#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include "Scene.h"

class Engine
{
public:
	void EngineStart();

	bool IsRunning() { return m_running; }
	void SetRunning(bool running) { m_running = running; }

private:
	bool m_running = true;
};

