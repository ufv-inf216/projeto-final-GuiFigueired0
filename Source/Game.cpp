// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fstream>
#include "SDL_image.h"
#include "Random.h"
#include "Game.h"
#include "AudioSystem.h"
#include "Actors/Actor.h"
#include "Actors/Player.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Components/ColliderComponents/AABBColliderComponent.h"
#include <Box2D/Box2D.h>
#include "Components/DrawComponents/DrawPolygonComponent.h"
#include "Scenes/Menu.h"
#include "Scenes/Level.h"

Game::Game(int windowWidth, int windowHeight)
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mLevelRunning(false)
        ,mUpdatingActors(false)
        ,mFadeState(FadeState::None)
        ,mSceneTransitionTime(0.0f)
        ,mWindowWidth(windowWidth)
        ,mWindowHeight(windowHeight)
        ,mCurrentLevel(0)
        ,mGameState(GameScene::Menu)
{
}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Fireboy and watergirl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, 0);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    mAudio = new AudioSystem();

    Random::Init();

    mTicksCount = SDL_GetTicks();

    InitializeScene();

    return true;
}

void Game::InitializeScene(){
    switch (mGameState)
    {
        case GameScene::Menu:
            mScene = new Menu(this);
            mScene->Load();
            break;

        case GameScene::Level:
            mWin = {0,0};
            mLevels.push_back(new Level(this, "../Assets/Maps/Map1_Layer1.csv", "../Assets/Maps/Map1_Objects.csv"));
            mScene = mLevels.back();
            mScene->Load();
            mLevelRunning = true;
            break;
        default:
            break;
    }
}

void Game::SetScene(GameScene gameState)
{
    // Stop all sounds
    mAudio->StopAllSounds();
    mFadeState = FadeState::FadeOut;
    mLevelRunning = false;

    // Handle scene transition
    mGameState = gameState;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                Quit();
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    for (auto actor : mActors)
    {
        actor->ProcessInput(state);
    }

    mAudio->ProcessInput(state);
    mScene->ProcessInput(state);
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (float)(SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    mAudio->Update(deltaTime);

    // Update all actors and pending actors
    UpdateActors(deltaTime);

    if (mFadeState == FadeState::FadeOut)
    {
        mSceneTransitionTime += deltaTime;
        if (mSceneTransitionTime >= SCENE_TRANSITION_TIME)
        {
            mSceneTransitionTime = 0.0f;
            mFadeState = FadeState::FadeIn;

            UnloadActors();
            InitializeScene();
        }
    }
    else if (mFadeState == FadeState::FadeIn)
    {
        mSceneTransitionTime += deltaTime;
        if (mSceneTransitionTime >= SCENE_TRANSITION_TIME)
        {
            mFadeState = FadeState::None;
            mSceneTransitionTime = 0.0f;
        }
    }

    if(mLevelRunning && mGameState == GameScene::Level)
        GetLevel()->UpdateLevel(deltaTime);

}

void Game::UpdateActors(float deltaTime)
{
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors)
    {
        delete actor;
    }
}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddDrawable(class DrawComponent *drawable)
{
    mDrawables.emplace_back(drawable);

    std::sort(mDrawables.begin(), mDrawables.end(),[](DrawComponent* a, DrawComponent* b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveDrawable(class DrawComponent *drawable)
{
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
}

void Game::AddCollider(class AABBColliderComponent* collider)
{
    mColliders.emplace_back(collider);
}

void Game::RemoveCollider(AABBColliderComponent* collider)
{
    auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
    mColliders.erase(iter);
}

void Game::GenerateOutput()
{
    // Set draw color to black
    SDL_SetRenderDrawColor(mRenderer, 107, 140, 255, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    for (auto drawable : mDrawables)
    {
        if (drawable->IsVisible())
        {
            drawable->Draw(mRenderer);
        }
    }
    if(showColliders())
        GetLevel()->DrawColliders(mRenderer);

    // Apply fade effect if changing scene
    if (mFadeState == FadeState::FadeOut)
    {
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, static_cast<Uint8>(255 * mSceneTransitionTime/SCENE_TRANSITION_TIME));
        SDL_RenderFillRect(mRenderer, nullptr);
    }
    else if (mFadeState == FadeState::FadeIn)
    {
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, static_cast<Uint8>(255 * (1.0f - mSceneTransitionTime/SCENE_TRANSITION_TIME)));
        SDL_RenderFillRect(mRenderer, nullptr);
    }

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

SDL_Texture* Game::LoadTexture(const std::string& texturePath) {
    SDL_Surface *image = IMG_Load(texturePath.c_str());
    if(!image)
    {
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, image);
    SDL_FreeSurface(image);
    if(!texture)
    {
        return nullptr;
    }

    return texture;
}

void Game::UnloadActors()
{
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    delete mScene;
}

void Game::Shutdown()
{
    UnloadActors();

    delete mAudio;
    IMG_Quit();

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
