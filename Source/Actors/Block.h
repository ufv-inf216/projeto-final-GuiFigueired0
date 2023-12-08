//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include "Actor.h"
#include "../Components/WorldBodyComponent.h"
#include "../AudioSystem.h"
#include <string>

class Block : public Actor
{
public:
    explicit Block(Game* game, const std::string &texturePath, int size = 32);
    WorldBodyComponent* GetBodyComponent() const { return mWorldBodyComponent; }
    void SetBodyComponent(WorldBodyComponent* bodyComponent) { mWorldBodyComponent = bodyComponent; }
    void OnUpdate(float deltaTime) override;
private:
    WorldBodyComponent* mWorldBodyComponent;
    SoundHandle mSoundHandle;
};
