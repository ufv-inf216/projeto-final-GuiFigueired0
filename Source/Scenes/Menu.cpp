//
// Created by Lucas N. Ferreira on 07/12/23.
//

#include "Menu.h"
#include "../Game.h"
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawTextComponent.h"

Menu::Menu(Game* game)
     :Scene(game)
{
    mMenuFont = game->GetFont("../Assets/Fonts/solemnit.ttf");
}

void Menu::Load()
{
    auto background = new Actor(GetGame());
    new DrawSpriteComponent(background, "../Assets/Sprites/Menu/TempleHallForest.jpg", 800, 800);

    // Press Space label
    auto *title = new Actor(GetGame());
    new DrawSpriteComponent(title, "../Assets/Sprites/Menu/PreloaderAssets.png", 465, 72);
    title->SetPosition(Vector2(mGame->GetWindowWidth()/2.0f - 232.5, mGame->GetWindowHeight()/2.0f - 36 - 60));

    auto *pressSpace = new Actor(GetGame());
    pressSpace->SetPosition(Vector2(mGame->GetWindowWidth()/2.0f, mGame->GetWindowHeight()/2.0f + 132.0f));
    new DrawTextComponent(pressSpace, "PRESS SPACE", mMenuFont, 200, 28, 64);

    mGame->GetSound()->PlaySound("Menu Music.mp3", true);
}

void Menu::ProcessInput(const Uint8 *keyState)
{
    if(keyState[SDL_SCANCODE_SPACE])
    {
        mGame->SetScene(Game::GameScene::Level);
    }
}