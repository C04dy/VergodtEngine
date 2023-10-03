#pragma once
#include "Node.h"
#include "Camera.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Sprite : public Node
{
public:
	void LoadImage(std::string FilePath);

	void DrawImage();

	void CleanImage();

	void SetCam(Camera* cam);

	void SetRenderer(SDL_Renderer* r);

private:
	SDL_Texture* m_texture;
	SDL_Renderer* m_renderer;
	Camera* m_cam;
	float m_w, m_h;
};