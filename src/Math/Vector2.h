#ifndef VECTOR2_H
#define VECTOR2_H

#pragma once

#include <box2d/b2_math.h>
#include "iostream"

class Vector2
{
public:
	Vector2() = default;

	Vector2(float xIn, float yIn) { x = xIn; y = yIn; }

	void Set(const Vector2& other);

	void operator=(const b2Vec2& other);

	Vector2 operator*(const Vector2& other);

	Vector2 operator*(float d);

	void operator+=(const Vector2& other);

	Vector2 operator+(const Vector2& other);

	Vector2 operator-(const Vector2& other);

	Vector2 operator/(float d);

	Vector2 AddToVec2(const Vector2& other);

	operator b2Vec2() const { return b2Vec2(x, y); }

	float x = 0.0f, y = 0.0f;
};

#endif