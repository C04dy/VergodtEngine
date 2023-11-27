#ifndef VECTOR2_H
#define VECTOR2_H

#pragma once

// Was using Vector2 = b2Vec2; but lua made me convert this into a class sooooo

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
};

#endif