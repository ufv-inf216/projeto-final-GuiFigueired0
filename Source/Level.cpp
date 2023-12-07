//
// Created by guilh on 05/12/2023.
//

#include "Level.h"
#include "Actors/Actor.h"
#include "Actors/Player.h"
#include "Actors/Block.h"
#include "Components/DrawComponents/DrawTileComponent.h"

// BOX2D
float TIME_STEP = 1.0f / 60.0f;
const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 3;

Level::Level(Game *game,std::string layerFileName, std::string objectsFileName)
        : mGame(game),
          mLayerFileName(std::move(layerFileName)),
          mObjectsFileName(std::move(objectsFileName)),
          mFireboy(nullptr),
          mWatergirl(nullptr),
          mWorld(nullptr),
          tf(new Transform()),
          mContactListener(new MyContactListener())
{}

void Level::InicializeLevel() {
    b2Vec2 gravity(0.0f, -40.0f);
    mWorld = new b2World(gravity);
    mWorld->SetContactListener(mContactListener);

    // Tiled
    auto* map = new Actor(GetGame());

    new DrawTileComponent(map, "../Assets/Maps/Map2.csv", "../Assets/Maps/finalBlocks.png", 800, 800, 32);
    LoadData("../Assets/Maps/Map2_Objects.csv");
}

void Level::UpdateLevel(float deltaTime) {
    GetWorld()->Step(deltaTime, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
    mWatergirl->GetBodyComponent()->Update();
    mWatergirl->SetPosition(mWatergirl->GetBodyComponent()->GetPosition());
    mFireboy->GetBodyComponent()->Update();
    mFireboy->SetPosition(mFireboy->GetBodyComponent()->GetPosition());
    for(auto &body: mBodies){
        body->Update();
    }

}

void Level::LoadData(const std::string& fileName)
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
            if(tiles[0] == "Player"){
                std::string name = tiles[1];
                if(name == "FireBoy")
                {
                    mFireboy = new class Player(GetGame(), line, GetWorld(), PlayerType::FireBoyHead, tf);
                    mFireboy->SetPosition(mFireboy->GetBodyComponent()->GetPosition());
                }
                else if(name == "WaterGirl")
                {
                    mWatergirl = new class Player(GetGame(), line, GetWorld(), PlayerType::WaterGirlHead, tf);
                    mWatergirl->SetPosition(mWatergirl->GetBodyComponent()->GetPosition());
                }
            } else if(tiles[1] == "Block"){
                auto block = new WorldBodyComponent(line, GetWorld(), tf);
                mBodies.push_back(block);
                auto myBlock = new Block(GetGame(), "", 45);
                mActors.push_back(myBlock);
            } else {
                mBodies.push_back(new WorldBodyComponent(line, GetWorld(), tf));
            }
        }
    }
}

void Level::DrawColliders(SDL_Renderer *renderer){

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for(int i=0; i<mBodies.size(); i++){
        if(mBodies[i]->GetClass() != "Box" && mBodies[i]->GetClass() != "Block") continue;
        auto collider = mBodies[i]->GetBody();
        auto groundBox = dynamic_cast<b2PolygonShape*>(collider->GetFixtureList()->GetShape());
        b2Vec2 worldSize = groundBox->m_vertices[2] - groundBox->m_vertices[0];
        auto position = tf->posWorldToMap(collider->GetPosition(), worldSize);
        Vector2 size(worldSize.x*32, worldSize.y*32);

        std::vector<Vector2> vertices;
        vertices.push_back(position);
        vertices.push_back(position + Vector2(0, size.y));
        vertices.push_back(position + Vector2(size.x, 0));
        vertices.push_back(position+size);

        // Render vertices as lines
        for(int i = 0; i < vertices.size() - 1; i++) {
            SDL_RenderDrawLine(renderer, (int)vertices[i].x,
                               (int)vertices[i].y,
                               (int)vertices[i+1].x,
                               (int)vertices[i+1].y);
        }

        // Close geometry
        SDL_RenderDrawLine(renderer, (int)vertices[vertices.size() - 1].x,
                           (int)vertices[vertices.size() - 1].y,
                           (int)vertices[0].x,
                           (int)vertices[0].y);
    }

    for(auto &v: mRamps){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for(int i = 0; i < v.size() - 1; i++) {
            SDL_RenderDrawLine(renderer, (int)v[i].x,
                               (int)v[i].y,
                               (int)v[i+1].x,
                               (int)v[i+1].y);
        }

        // Close geometry
        SDL_RenderDrawLine(renderer, (int)v[v.size() - 1].x,
                           (int)v[v.size() - 1].y,
                           (int)v[0].x,
                           (int)v[0].y);
    }
}