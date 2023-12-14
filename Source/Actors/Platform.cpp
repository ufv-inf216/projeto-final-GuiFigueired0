//
// Created by guilh on 14/12/2023.
//

#include "Platform.h"

Platform::Platform(Game *game, const std::string &line, b2World *world, Transform *transform, float velocity)
    : Actor(game), mWorldBodyComponent(new WorldBodyComponent(line, world, transform, this)), mVelocity(velocity)
    {
    auto tiles = CSVHelper::Split(line);
    id = tiles[1][1] - '0';
}

void Platform::OnUpdate() {
    mWorldBodyComponent->GetBody()->SetLinearVelocity(b2Vec2(0.0f, mVelocity));
}