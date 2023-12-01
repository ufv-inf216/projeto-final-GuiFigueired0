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
#include "Actors/Block.h"
#include "Actors/Goomba.h"
#include "Actors/Spawner.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Components/ColliderComponents/AABBColliderComponent.h"
#include "Components/DrawComponents/DrawTileComponent.h"
#include <Box2D/Box2D.h>
#include "Components/DrawComponents/DrawPolygonComponent.h"

const int LEVEL_WIDTH = 213;
const int LEVEL_HEIGHT = 14;
const int TILE_SIZE = 32;
const float SPAWN_DISTANCE = 600.0f;

// BOX2D
float TIME_STEP = 1.0f / 60.0f;
const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 3;


Game::Game(int windowWidth, int windowHeight)
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mUpdatingActors(false)
        ,mWindowWidth(windowWidth)
        ,mWindowHeight(windowHeight)
        ,mWorld(nullptr)
{
    mWorldColliders.clear();
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

    mAudio = new AudioSystem(8);
    mAudio->PlaySound("Level Music.mp3",true);

    Random::Init();

    mTicksCount = SDL_GetTicks();

    // Init all game actors
    InitializeActors();

    return true;
}

void Game::InitializeActors()
{
    //LoadLevel("../Assets/Levels/LevelTeste.txt", LEVEL_WIDTH, LEVEL_HEIGHT);

    b2Vec2 gravity(0.0f, -40.0f);
    mWorld = new b2World(gravity);

    // Tiled
    auto* map = new Actor(this);
    ///*
    new DrawTileComponent(map, "../Assets/Maps/Map1_Layer1.csv", "../Assets/Maps/myBlocks.png", 576, 576, 32);
    new DrawTileComponent(map, "../Assets/Maps/Map1_Layer2.csv", "../Assets/Maps/myBlocks.png", 576, 576, 32);

    LoadData("../Assets/Maps/Map1_Objects.csv");
    //*/
    //new DrawTileComponent(map, "../Assets/Maps/MapTeste.csv", "../Assets/Maps/myBlocks.png", 576, 576, 32);
    //LoadData("../Assets/Maps/MapTeste_Objects.csv");
}

void Game::LoadLevel(const std::string& levelPath, const int width, const int height)
{
    std::ifstream file;
    file.open(levelPath);
    for(int i=0; i<height; i++){
        std::string line;
        std::getline(file, line);
        for(int j=0; j<width; j++){
            if(line[j] >= 'A' && line[j] <= 'I'){
                auto block = new Block(this, std::string(1, line[j]));
                block->SetPosition(Vector2((float)j*32, (float)i*32));
            } else if(line[j] == 'Y'){
                auto spawner = new Spawner(this, SPAWN_DISTANCE);
                spawner->SetPosition(Vector2((float)j*32, (float)i*32));
            }
        }
    }
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

    // Box2D
    GetWorld()->Step(deltaTime/2.0f, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
    mAgua->GetBodyComponent()->Update();
    mAgua->SetPosition(mAgua->GetBodyComponent()->GetPosition());
    //printf("%4.2f %4.2f \n", pos.x, pos.y);


    // Update all actors and pending actors
    UpdateActors(deltaTime);

    // Update camera position
    UpdateCamera();
}

void Game::UpdateCamera()
{
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

    SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
    for(auto &collider: mWorldColliders){
        auto groundBox = dynamic_cast<b2PolygonShape*>(collider->GetFixtureList()->GetShape());
        b2Vec2 worldSize = groundBox->m_vertices[2] - groundBox->m_vertices[0];
        auto position = tf.posWorldToMap(collider->GetPosition(), worldSize);
        Vector2 size(worldSize.x*32, worldSize.y*32);

        std::vector<Vector2> vertices;
        vertices.push_back(position);
        vertices.push_back(position + Vector2(0, size.y));
        vertices.push_back(position + Vector2(size.x, 0));
        vertices.push_back(position+size);

        // Render vertices as lines
        for(int i = 0; i < vertices.size() - 1; i++) {
            SDL_RenderDrawLine(mRenderer, (int)vertices[i].x,
                               (int)vertices[i].y,
                               (int)vertices[i+1].x,
                               (int)vertices[i+1].y);
        }

        // Close geometry
        SDL_RenderDrawLine(mRenderer, (int)vertices[vertices.size() - 1].x,
                           (int)vertices[vertices.size() - 1].y,
                           (int)vertices[0].x,
                           (int)vertices[0].y);
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

void Game::LoadData(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        SDL_Log("Failed to load paths: %s", fileName.c_str());
    }

    int row = 0;

    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);

        if(!line.empty())
        {
            auto tiles = CSVHelper::Split(line);

            if(tiles[0] == "Type") {
                continue;
            }

            float x = std::stof(tiles[1]);
            float y = std::stof(tiles[2]);
            Vector2 pos(x, y);
            float width = std::stof(tiles[3]);
            float height = std::stof(tiles[4]);
            Vector2 size(width, height);

            if(tiles[0].empty())
            {
                //mPlayer = new Player(this);
                //mPlayer->SetPosition(Vector2(x + width/2.0f, y + height/2.0));
            }
            else if(tiles[0] == "Floor" || tiles[0] == "Box")
            {
                b2BodyDef groundBodyDef;
                b2PolygonShape groundBox;
                groundBodyDef.position = tf.posMapToWorld(pos, size);
                b2Body* groundBody = GetWorld()->CreateBody(&groundBodyDef);
                groundBox.SetAsBox(tf.sizeMapToWorld(width), tf.sizeMapToWorld(height));
                groundBody->CreateFixture(&groundBox, 0.0f);
                mWorldColliders.push_back(groundBody);
            }
            else if(tiles[0] == "Player")
            {
                b2BodyDef bodyDef;
                bodyDef.type = b2_dynamicBody;
                b2Vec2 worldPos = tf.posMapToWorld(pos, size);;
                bodyDef.position = worldPos;
                b2Body* body = GetWorld()->CreateBody(&bodyDef);

                b2PolygonShape dynamicBox;
                b2Vec2 worldSize(tf.sizeMapToWorld(width), tf.sizeMapToWorld(height));
                dynamicBox.SetAsBox(worldSize.x, worldSize.y);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.3f;
                body->CreateFixture(&fixtureDef);

                mAgua = new Player(this, body, PlayerType::FireBoyHead, &tf);
                mAgua->SetPosition(tf.posWorldToMap(worldPos, worldSize));
                mPlayerBody = body;
            }
        }
    }
}

void Game::Shutdown()
{
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
