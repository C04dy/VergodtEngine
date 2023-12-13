#ifndef VECTOR2_H
#define VECTOR2_H

#pragma once

#include <box2d/b2_math.h>

class Vector2 : public b2Vec2
{
public:
    Vector2() = default;

    Vector2(float xIn, float yIn) : b2Vec2(xIn, yIn) {};

	void SetX(float sx){
		x = sx;
	}

	float GetX() const{
		return x;
	}

	void SetY(float sy){
		y = sy;
	}

	float GetY() const{
		return y;
	}

	void operator=(const b2Vec2& other){
		x = other.x;
		y = other.y;
	}
};

#endif