#pragma once

#include "Sprite.h"

class Texture {
public:
	Sprite* s;

	void ChangeTexture(const std::string &FilePath) { s->ChangeTexture(FilePath); }
};