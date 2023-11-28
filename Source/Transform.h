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

    b2Vec2 posMapToWorld(Vector2 pos) const;
    Vector2 posWorldToMap(b2Vec2 pos) const;

    float sizeMapToWorld(float x) const;

private:
    float tileSize;
    float mapWidth;
    float mapHeight;
};


#endif
