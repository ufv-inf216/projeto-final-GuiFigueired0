//
// Created by guilh on 28/11/2023.
//

#include "Transform.h"

Transform::Transform()
    : tileSize(32.0), mapHeight(800) {}

b2Vec2 Transform::posMapToWorld(Vector2 pos, Vector2 size) const {
    float x = (pos.x + size.x*0.5f)/tileSize;
    float y = (mapHeight - (pos.y + size.y*0.5f))/tileSize;
    return {x, y};
}

Vector2 Transform::posWorldToMap(b2Vec2 pos, b2Vec2 size) const {
    Vector2 pixels_pos(pos.x * 32, mapHeight - pos.y*32);
    Vector2 pixels_size(size.x * 32, size.y*32);
    float x = pixels_pos.x - pixels_size.x*0.5f;
    float y = pixels_pos.y - pixels_size.y*0.5f;
    return Vector2(x, y);
}

float Transform::sizeMapToWorld(float x) const {
    return x/(tileSize*2);
}

float Transform::sizeWorldToMap(float x) const {
    return x*tileSize;
}

b2Vec2 Transform::pointMapToWorld(float x, float y) const {
    return { x/tileSize, (mapHeight - y)/tileSize };
}

Vector2 Transform::pointWorldToMap(b2Vec2 v) const {
    return Vector2(v.x * tileSize, mapHeight - v.y * tileSize);
}