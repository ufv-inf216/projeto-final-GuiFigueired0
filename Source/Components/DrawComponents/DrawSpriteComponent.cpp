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
{
    mSpriteSheetSurface = GetGame()->LoadTexture(texturePath);
}

void DrawSpriteComponent::Draw(SDL_Renderer *renderer)
{
    Vector2 pos = GetOwner()->GetPosition();
    Vector2 camera = GetGame()->GetCameraPos();
    SDL_Rect dstrect{
            (int)pos.x - (int)camera.x,
            (int)pos.y - (int)camera.y,
            mWidth,
            mHeight,
    };
    SDL_RenderCopyEx(
            renderer,
            mSpriteSheetSurface,
            nullptr, &dstrect,
            mOwner->GetRotation(),
            nullptr,
            SDL_RendererFlip::SDL_FLIP_HORIZONTAL
            );

}