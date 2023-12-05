//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawAnimatedComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Json.h"
#include <fstream>

#include <iostream>

DrawAnimatedComponent::DrawAnimatedComponent(class Actor* owner, const std::string &spriteSheetPath, const std::string &spriteSheetData, int drawOrder)
        :DrawSpriteComponent(owner, spriteSheetPath, 0, 0, drawOrder)
{
    LoadSpriteSheet(spriteSheetPath, spriteSheetData);
}

DrawAnimatedComponent::~DrawAnimatedComponent()
{
    for (const auto& rect : mSpriteSheetData)
    {
        delete rect;
    }
    mSpriteSheetData.clear();
}

void DrawAnimatedComponent::LoadSpriteSheet(const std::string& texturePath, const std::string& dataPath)
{
    // Load sprite sheet texture
    mSpriteSheetSurface = mOwner->GetGame()->LoadTexture(texturePath);

    // Load sprite sheet data
    std::ifstream spriteSheetFile(dataPath);
    nlohmann::json spriteSheetData = nlohmann::json::parse(spriteSheetFile);

    SDL_Rect* rect = nullptr;
    for(const auto& frame : spriteSheetData["frames"]) {

        int x = frame["frame"]["x"].get<int>();
        int y = frame["frame"]["y"].get<int>();
        int w = frame["frame"]["w"].get<int>();
        int h = frame["frame"]["h"].get<int>();
        rect = new SDL_Rect({x, y, w, h});

        mSpriteSheetData.emplace_back(rect);
    }
}

void DrawAnimatedComponent::Draw(SDL_Renderer *renderer)
{
    auto spriteIdx =  mAnimations[mAnimName][(int)mAnimTimer];
    Vector2 pos = GetOwner()->GetPosition();

    SDL_Rect dstrect{
            (int)pos.x - (int)GetGame()->GetCameraPos().x-50,
            (int)pos.y-50,
            mSpriteSheetData[spriteIdx]->w,
            mSpriteSheetData[spriteIdx]->h,
    };
    auto rotation = mOwner->GetRotation();
    auto angle = rotation;
    if(mAnimName == "Running")
        angle = abs(mOwner->GetAngle());
    SDL_RenderCopyEx(
            renderer,
            mSpriteSheetSurface,
            mSpriteSheetData[spriteIdx],
            &dstrect,
            angle,
            nullptr,
            (rotation == 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL
    );
}

void DrawAnimatedComponent::Update(float deltaTime)
{
    if(mIsPaused) return;

    mAnimTimer += mAnimFPS*deltaTime*(1+(GetOwner()->GetForward().x/3000.0f));

    while(mAnimTimer >= (float)mAnimations[mAnimName].size())
        mAnimTimer -= (float)mAnimations[mAnimName].size();
}

void DrawAnimatedComponent::SetAnimation(const std::string& name)
{
    mAnimName = name;
    Update(0);
}

void DrawAnimatedComponent::AddAnimation(const std::string& name, const std::vector<int>& spriteNums)
{
    mAnimations.emplace(name, spriteNums);
}
