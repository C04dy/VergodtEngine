#pragma once
#include <iostream>
#include <SDL2/SDL.h>

class b2World;

class BaseScene
{
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void Draw();
	virtual void Clean();

	void SetRenderer(SDL_Renderer* r) { Renderer = r; }
	void SetPhysicsWorld(b2World* w) { PhysicsWorld = w; }

	std::string GetGameName() { return m_gamename; }
	void SetGameName(std::string gamename) { m_gamename = gamename; }

	int GetWindowWidth() { return m_windowwidth; }
	int GetWindowHeight() { return m_windowheight; }

	void SetWindowsWidth(int w) { m_windowwidth = w; }
	void SetWindowsHeight(int h) { m_windowheight = h; }

protected:
	SDL_Renderer* Renderer;

	b2World* PhysicsWorld;
private:
	std::string m_gamename = "VergodtEngineGame";

	int m_windowwidth = 1280, m_windowheight = 720;

};