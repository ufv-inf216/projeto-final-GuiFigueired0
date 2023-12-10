//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Block.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../AudioSystem.h"

#define EPS 1e-9

Block::Block(Game* game, const std::string &texturePath, int x, int y)
        :Actor(game)
        ,mDoorType(texturePath)
{
    new DrawSpriteComponent(this, "../Assets/Sprites/"+texturePath+".png", x, y, 100);
}

void Block::OnUpdate(float deltaTime)
{
    if (GetGame()->GetSound()->GetSoundState(mSoundHandle) != SoundState::Playing && abs(mWorldBodyComponent->GetVelocity().x) > EPS) {
        mSoundHandle = GetGame()->GetSound()->PlaySound("Platform corrupt.mp3");
    }
    if (GetGame()->GetSound()->GetSoundState(mSoundHandle) == SoundState::Playing && abs(mWorldBodyComponent->GetVelocity().x) < EPS) {
        GetGame()->GetSound()->StopSound(mSoundHandle);
    }
    if(GetGame()->GetStateWin()){
        if(mDoorType == "Temple/DoorFire")
        {
            mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Temple/TempleAssets.png", "../Assets/Sprites/Temple/TempleAssets.json", 1/1.8f, Vector2(13,6), 101);
            mDrawComponent->AddAnimation("OpenDoor", std::vector<int>{3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24});
            mDrawComponent->SetAnimation("OpenDoor");
            mDrawComponent->SetAnimFPS(15.0f);
            mDoorType = "";
        }
        if(mDoorType == "Temple/DoorWater")
        {
            mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Temple/TempleAssets.png", "../Assets/Sprites/Temple/TempleAssets.json", 1/1.8f, Vector2(13,6), 101);
            mDrawComponent->AddAnimation("OpenDoor", std::vector<int>{25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46});
            mDrawComponent->SetAnimation("OpenDoor");
            mDrawComponent->SetAnimFPS(15.0f);
            mDoorType = "";
        }
    }
}