#include "Sprite.h"

Sprite::~Sprite()
{
    SDL_DestroyTexture(m_Texture);
}

void Sprite::InitSprite(const std::string& FilePath, SDL_Renderer* Renderer)
{
    m_Renderer = Renderer;
    NodeType = Node::Type::SPRITE;
    LoadImage(FilePath);
}

void Sprite::LoadImage(const std::string& FilePath)
{
    SDL_Surface* surface = IMG_Load(FilePath.c_str());

    if (surface == nullptr)
    {
        std::cout << "Failed to load texture: " << FilePath.c_str() << ",";
        throw std::runtime_error(SDL_GetError());
    }

    m_Width = surface->w;
    m_Heigth = surface->h;
    
    m_Texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
    if (m_Texture == nullptr)
    {
        std::cout << "Failed to create texture from surface: ";
        throw std::runtime_error(SDL_GetError());
    }

    SDL_DestroySurface(surface);
}

void Sprite::DrawImage(Camera* Camera)
{
    SDL_FRect source_rec = { 0, 0, m_Width, m_Heigth };

    SDL_FRect destination_rect = { (Position.x - m_Width * Size.x / 2) - Camera->Position.x
                        , (Position.y - m_Heigth * Size.y / 2) - Camera->Position.y
                        , m_Width * Size.x
                        , m_Heigth * Size.y};
    SDL_RenderTextureRotated(m_Renderer, m_Texture, &source_rec, &destination_rect, Angle, nullptr, SDL_FLIP_NONE);
}

void Sprite::ChangeTexture(const std::string& FilePath)
{
    SDL_DestroyTexture(m_Texture);
    LoadImage(FilePath);
}