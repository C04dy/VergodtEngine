#include "Sprite.h"

void Sprite::InitSprite(const std::string& FilePath, Camera* Cam, SDL_Renderer* Renderer) {
    m_renderer = Renderer;
    Type = NodeType::SPRITE;
    SetCam(Cam);
    LoadImage(FilePath);
}

void Sprite::DeleteTexture(){
    SDL_DestroyTexture(m_texture);
}

void Sprite::LoadImage(const std::string& FilePath){
    SDL_Surface* surface = IMG_Load(FilePath.c_str());

    if(surface == nullptr){
        std::cout << "Failed to load texture: " << FilePath.c_str() << ",";
        throw std::runtime_error(SDL_GetError());
    }

    m_w = surface->w;
    m_h = surface->h;
    
    m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if(m_texture == nullptr){
        std::cout << "Failed to create texture from surface: ";
        throw std::runtime_error(SDL_GetError());
    }

    SDL_DestroySurface(surface);
}

void Sprite::DrawImage(){
    SDL_FRect srcRect = { 0, 0, m_w, m_h };
    SDL_FRect dstRect = { (Position.x - m_w * Size.x / 2) - m_cam->Position.x
                        , (Position.y - m_h * Size.y / 2) - m_cam->Position.y
                        , m_w * Size.x
                        , m_h * Size.y};
    SDL_RenderTextureRotated(m_renderer, m_texture, &srcRect, &dstRect, Angle, nullptr, SDL_FLIP_NONE);
}

void Sprite::SetCam(Camera* Cam){
	m_cam = Cam;
}

void Sprite::ChangeTexture(const std::string& FilePath) {
    DeleteTexture();
    LoadImage(FilePath);
}