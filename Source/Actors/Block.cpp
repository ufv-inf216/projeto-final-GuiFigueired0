//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Block.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../AudioSystem.h"

#define EPS 1e-9

Block::Block(Game* game, const std::string &texturePath, int size)
        :Actor(game)
{
    new DrawSpriteComponent(this, "../Assets/Sprites/Blocks/Block"+texturePath+".png", size, size);
}

void Block::OnUpdate(float deltaTime)
{
    if(GetGame()->GetSound()->GetSoundState(mSoundHandle) != SoundState::Playing && abs(mWorldBodyComponent->GetVelocity().x) > EPS)
    {
        mSoundHandle = GetGame()->GetSound()->PlaySound("Platform corrupt.mp3");
    }
    if(GetGame()->GetSound()->GetSoundState(mSoundHandle) == SoundState::Playing && abs(mWorldBodyComponent->GetVelocity().x) < EPS)
    {
        GetGame()->GetSound()->StopSound(mSoundHandle);
    }
}