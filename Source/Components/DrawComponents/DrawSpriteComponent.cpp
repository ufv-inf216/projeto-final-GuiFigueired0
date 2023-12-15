//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawSpriteComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawSpriteComponent::DrawSpriteComponent(class Actor* owner, const std::string &texturePath, const int width, const int height, const int drawOrder)
        :DrawComponent(owner, drawOrder)
        ,mWidth(width)
        ,mHeight(height)
        ,mFlip(false)
{
    mSpriteSheetSurface = GetGame()->LoadTexture(texturePath);

    if(texturePath == "../Assets/Sprites/Menu/PreloaderAssets.png" ||
       texturePath == "../Assets/Sprites/Menu/Paused.png")
        mFlip = true;
}

void DrawSpriteComponent::Draw(SDL_Renderer *renderer)
{
    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

    if(mFlip)
        flip = SDL_FLIP_NONE;

    if(GetOwner()->GetState() != ActorState::Invisible) {
        Vector2 pos = GetOwner()->GetPosition();
        SDL_Rect dstrect{
                (int) pos.x,
                (int) pos.y,
                mWidth,
                mHeight,
        };
        SDL_RenderCopyEx(
                renderer,
                mSpriteSheetSurface,
                nullptr, &dstrect,
                mOwner->GetRotation(),
                nullptr,
                flip
        );
    }
}