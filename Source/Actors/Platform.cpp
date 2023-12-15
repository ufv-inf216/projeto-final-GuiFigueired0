//
// Created by guilh on 14/12/2023.
//

#include "Platform.h"
#include <algorithm>

Platform::Platform(Game *game, const std::string &line, b2World *world, Transform *transform)
    : Actor(game), tf(transform)
    {
    mWorldBodyComponent = new WorldBodyComponent(line, world, transform, this);
    auto tiles = CSVHelper::Split(line);
    id = std::stoi(tiles[1]);
    auto w = std::stoi(tiles[4]);
    auto h = std::stoi(tiles[5]);
    mBottomLimit = std::stof(tiles[6]);
    mTopLimit = std::stof(tiles[7]);
    mVelocity = std::stof(tiles[8]);

    //new DrawSpriteComponent(this, "../Assets/Sprites/"+texturePath+".png", x, y, 100);
    new DrawSpriteComponent(this, "../Assets/Sprites/Platforms/PlatformYellow.png", w, h, 100);
}

void Platform::OnUpdate() {
    if(mWorldBodyComponent->GetPosition().y < mTopLimit ||
        mWorldBodyComponent->GetPosition().y + mWorldBodyComponent->GetSize().y > mBottomLimit)
        mVelocity = -mVelocity;
    mWorldBodyComponent->GetBody()->SetLinearVelocity(b2Vec2(0.0f, mVelocity));
}

void Platform::SetColision() {
    mVelocity = -mVelocity;
    mWorldBodyComponent->GetBody()->SetLinearVelocity(b2Vec2(0.0f, mVelocity));
}