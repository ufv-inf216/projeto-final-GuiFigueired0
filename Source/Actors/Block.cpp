//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Block.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Block::Block(Game* game, const std::string &texturePath, int size)
        :Actor(game)
{
    new DrawSpriteComponent(this, "../Assets/Sprites/Blocks/Block"+texturePath+".png", size, size);
}
