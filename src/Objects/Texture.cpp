#include "Texture.h"
#include "Sprite.h"

void Texture::ChangeTexture(const std::string &FilePath) {
    ((Sprite*)s)->ChangeTexture(FilePath);
}