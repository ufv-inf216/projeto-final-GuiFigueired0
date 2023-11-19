//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Block.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Block::Block(Game* game, const std::string &texturePath)
        :Actor(game)
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 1.1 (1 linha): Crie o `AABBColliderComponent` com dimensões `(0,0,32,32)` e tipo de colisão `ColliderLayer::Blocks`.
    new AABBColliderComponent(this, 0, 0, 32, 32, ColliderLayer::Blocks);
    // TODO 1.2 (1 linha): Crie o `DrawSpriteComponent` com textura `texturePath` (passada com parâmetro) e dimensões `(32,32)`.
    new DrawSpriteComponent(this, "../Assets/Sprites/Blocks/Block"+texturePath+".png", 32, 32);
}
