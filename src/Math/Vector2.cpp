#include "Vector2.h"

void Vector2::operator=(const b2Vec2& other){
    x = other.x;
    y += other.y;
}

void Vector2::operator+=(const Vector2& other){
    x += other.x; 
    y += other.y;
}

Vector2 Vector2::operator/(float d) {
    return Vector2(x / d, y / d);
}

Vector2 Vector2::AddToVec2(const Vector2 &other) {
    return Vector2(x + other.x, y + other.y);
}