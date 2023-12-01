//
// Created by guilh on 28/11/2023.
//

#ifndef FOGO_E_AGUA_TRANSFORM_H
#define FOGO_E_AGUA_TRANSFORM_H

#include <SDL.h>
#include <Box2D/Box2D.h>
#include "Math.h"

class Transform {
public:
    Transform();

    b2Vec2 posMapToWorld(Vector2 pos, Vector2 size) const;
    Vector2 posWorldToMap(b2Vec2 pos, b2Vec2 size) const;

    float sizeMapToWorld(float x) const;
    float sizeWorldToMap(float x) const;

private:
    float tileSize;
    float mapHeight;
};


#endif
