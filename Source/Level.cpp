//
// Created by guilh on 05/12/2023.
//

#include "Level.h"
#include "Actors/Actor.h"
#include "Actors/Player.h"
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
          tf(new Transform())
{}

void Level::InicializeLevel() {
    b2Vec2 gravity(0.0f, -40.0f);
    mWorld = new b2World(gravity);

    // Tiled
    auto* map = new Actor(GetGame());
    ///*
    new DrawTileComponent(map, "../Assets/Maps/Map1_Layer1.csv", "../Assets/Maps/myBlocks.png", 800, 800, 32);

    LoadData("../Assets/Maps/Map1_Objects.csv");
}

void Level::UpdateLevel(float deltaTime) {
    GetWorld()->Step(deltaTime, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
    mWatergirl->GetBodyComponent()->Update();
    mWatergirl->SetPosition(mWatergirl->GetBodyComponent()->GetPosition());
    mFireboy->GetBodyComponent()->Update();
    mFireboy->SetPosition(mFireboy->GetBodyComponent()->GetPosition());
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
            float x = std::stof(tiles[1]);
            float y = std::stof(tiles[2]);
            Vector2 pos(x, y);

            if(tiles[0] == "Ramp") {
                // Defina os vértices para um triângulo (em metros)
                b2Vec2 vertices[3];
                vertices[0] = tf->pointMapToWorld(std::stof(tiles[3])+x, std::stof(tiles[4])+y);
                vertices[1] = tf->pointMapToWorld(std::stof(tiles[5])+x, std::stof(tiles[6])+y);
                vertices[2] = tf->pointMapToWorld(std::stof(tiles[7])+x, std::stof(tiles[8])+y);

                b2BodyDef rampBodyDef;
                b2Body* rampBody = GetWorld()->CreateBody(&rampBodyDef);
                b2PolygonShape shape;
                shape.Set(vertices, 3);
                rampBody->CreateFixture(&shape, 1.0f);
                std::vector<Vector2> v = { tf->pointWorldToMap(vertices[0]), tf->pointWorldToMap(vertices[1]), tf->pointWorldToMap(vertices[2]) };
                mRamps.emplace_back(v);
            } else {
                float width = std::stof(tiles[3]);
                float height = std::stof(tiles[4]);
                Vector2 size(width, height);

                if(tiles[0] == "Floor" || tiles[0] == "Box")
                {
                    b2BodyDef groundBodyDef;
                    b2PolygonShape groundBox;
                    groundBodyDef.position = tf->posMapToWorld(pos, size);
                    b2Body* groundBody = GetWorld()->CreateBody(&groundBodyDef);
                    groundBox.SetAsBox(tf->sizeMapToWorld(width), tf->sizeMapToWorld(height));
                    groundBody->CreateFixture(&groundBox, 0.0f);
                    mWorldColliders.push_back(groundBody);
                }
                else if(tiles[0] == "Player")
                {
                    b2BodyDef bodyDef;
                    bodyDef.type = b2_dynamicBody;
                    b2Vec2 worldPos = tf->posMapToWorld(pos, size);;
                    bodyDef.position = worldPos;
                    b2Body* body = GetWorld()->CreateBody(&bodyDef);

                    b2PolygonShape dynamicBox;
                    b2Vec2 worldSize(tf->sizeMapToWorld(width), tf->sizeMapToWorld(height));
                    dynamicBox.SetAsBox(worldSize.x, worldSize.y);

                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &dynamicBox;
                    fixtureDef.density = 1.0f;
                    fixtureDef.friction = 0.0f;
                    body->CreateFixture(&fixtureDef);

                    std::string name = tiles[5];
                    if(name == "FireBoy")
                    {
                        mFireboy = new Player(GetGame(), body, PlayerType::FireBoyHead, tf);
                        mFireboy->SetPosition(tf->posWorldToMap(worldPos, worldSize));
                    }
                    else if(name == "WaterGirl")
                    {
                        mWatergirl = new Player(GetGame(), body, PlayerType::WaterGirlHead, tf);
                        mWatergirl->SetPosition(tf->posWorldToMap(worldPos, worldSize));
                    }
                }
            }
        }
    }
}

void Level::DrawColliders(SDL_Renderer *renderer){

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for(auto &collider: mWorldColliders){
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