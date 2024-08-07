#pragma once
#include "Node.h"
#include "Camera.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class Sprite : public Node
{
public:
	void InitSprite(const std::string& FilePath, Camera* Cam, SDL_Renderer* Renderer);

	void DeleteTexture();

	void LoadImage(const std::string& FilePath);

	void DrawImage(); 

	void SetCam(Camera* Cam);

	void ChangeTexture(const std::string& FilePath);

	class Texture {
	public:
		Node* s;

		void ChangeTexture(const std::string &FilePath){
			((Sprite*)s)->ChangeTexture(FilePath);
		}
	private:
	};

	Texture texture;
private:
	SDL_Texture* m_texture = nullptr;
	
	SDL_Renderer* m_renderer = nullptr;
	
	Camera* m_cam = nullptr;
	
	float m_w, m_h;
};