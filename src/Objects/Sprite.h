#pragma once
#include "Node.h"
#include "Camera.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class Sprite : public Node
{
public:
	~Sprite();

	void InitSprite(const std::string& FilePath, SDL_Renderer* Renderer);

	void LoadImage(const std::string& FilePath);

	void DrawImage(Camera* Camera);

	void ChangeTexture(const std::string& FilePath);
private:
	SDL_Texture* m_Texture = nullptr;
	
	SDL_Renderer* m_Renderer = nullptr;
		
	float m_Width, m_Heigth;
};