//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Block.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../AudioSystem.h"

Block::Block(Game* game, const std::string &texturePath, int size)
        :Actor(game),
        mAudioTime(0)
{
    new DrawSpriteComponent(this, "../Assets/Sprites/Blocks/Block"+texturePath+".png", size, size);
    mSoundHandle = GetGame()->GetSound()->PlaySound("Platform corrupt.mp3");
    GetGame()->GetSound()->StopSound(mSoundHandle);
}

void Block::OnUpdate(float deltaTime)
{
    if(GetGame()->GetSound()->GetSoundState(mSoundHandle) != SoundState::Playing && abs(mWorldBodyComponent->GetVelocity().x) > 0)
    {
        mSoundHandle = GetGame()->GetSound()->PlaySound("Platform corrupt.mp3");
    }
    if(GetGame()->GetSound()->GetSoundState(mSoundHandle) == SoundState::Playing && mWorldBodyComponent->GetVelocity().x == 0)
    {
        GetGame()->GetSound()->StopSound(mSoundHandle);
    }
}