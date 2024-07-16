#ifndef VECTOR2_H
#define VECTOR2_H

#pragma once

#include <box2d/b2_math.h>
#include "iostream"

class Vector2
{
public:
    Vector2() = default;

    Vector2(float xIn, float yIn)  { x = xIn; y = yIn; }

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

	Vector2 operator/(float d) {
		return Vector2(x / d, y / d);
	}

	operator b2Vec2() const { return b2Vec2(x, y); }

	float x = 0.0f, y = 0.0f;
};

#endif