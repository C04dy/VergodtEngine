#include "Sprite.h"

void Sprite::LoadImage(std::string FilePath)
{
    SDL_Surface* surface = IMG_Load(FilePath.c_str());

    if (surface == nullptr) {
        SDL_Log("Failed to load texture: %s, %s", FilePath.c_str(), SDL_GetError());
        return;
    }

    m_w = surface->w;
    m_h = surface->h;

    m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (m_texture == nullptr) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        return;
    }
}

void Sprite::DrawImage()
{
    SDL_Rect srcRect = { 0, 0, m_w, m_h };
    SDL_Rect dstRect = { (Transform.Position.x - m_w * Transform.Size.x / 2) + m_cam->Transform.Position.x - LocalPosition.x
                             , (Transform.Position.y - m_h * Transform.Size.y / 2) - m_cam->Transform.Position.y - LocalPosition.y
                                    , m_w * Transform.Size.x, m_h * Transform.Size.y};
    SDL_RenderCopyEx(m_renderer, m_texture, &srcRect, &dstRect, Transform.Angle, nullptr, SDL_FLIP_NONE);
}

void Sprite::CleanImage()
{
    SDL_DestroyTexture(m_texture);
}

void Sprite::SetCam(Camera* cam)
{
	m_cam = cam;
}

void Sprite::SetRenderer(SDL_Renderer* r)
{
    m_renderer = r;
}