//
// Created by guilh on 28/11/2023.
//

#include "Transform.h"

Transform::Transform()
    : tileSize(32.0), mapWidth(25.0), mapHeight(25.0) {}

b2Vec2 Transform::posMapToWorld(Vector2 pos) const {
    float x = mapWidth - pos.x/tileSize;
    float y = mapHeight - pos.y/tileSize;
    return {x, y};
}

Vector2 Transform::posWorldToMap(b2Vec2 pos) const {
    float x = -1*(pos.x - mapHeight)*tileSize;
    float y = -1*(pos.x - mapHeight)*tileSize;
    return Vector2(x, y);
}

float Transform::sizeMapToWorld(float x) const {
    return x/(tileSize*2);
}