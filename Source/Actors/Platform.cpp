//
// Created by guilh on 14/12/2023.
//

#include "Platform.h"
#include <algorithm>

Platform::Platform(Game *game, const std::string &line, b2World *world, Transform *transform, float velocity)
    : Actor(game), mVelocity(velocity)
    {
    mWorldBodyComponent = new WorldBodyComponent(line, world, transform, this);
    hPos = mWorldBodyComponent->GetBody()->GetPosition().y;
    auto tiles = CSVHelper::Split(line);
    id = tiles[1][1] - '0';
}

void Platform::OnUpdate() {
    if(std::abs(mWorldBodyComponent->GetBody()->GetPosition().y - hPos) >= 2.0f)
        mVelocity = -mVelocity;
    mWorldBodyComponent->GetBody()->SetLinearVelocity(b2Vec2(0.0f, mVelocity));
}