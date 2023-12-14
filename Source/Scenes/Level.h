//
// Created by guilh on 05/12/2023.
//

#ifndef FOGO_E_AGUA_LEVEL_H
#define FOGO_E_AGUA_LEVEL_H

#include <SDL.h>
#include <vector>
#include <string>
#include "../Math.h"
#include "../CSV.h"
#include "box2d/box2d.h"
#include "../Transform.h"
#include "../Game.h"
#include "../Actors/Player.h"
#include "../MyContactListener.h"
#include <fstream>
#include "Scene.h"
#include "../Actors/Platform.h"

class Level : public Scene {
public:
    Level(Game* game, std::string layerFileName, std::string objectsFileName);
    ~Level();

    void InicializeLevel();
    void UpdateLevel(float deltaTime);
    void DrawColliders(SDL_Renderer *renderer);
    class Game* GetGame() { return mGame; }
    std::vector<class Actor*>* GetActors() { return &mActors; }

    // Tiled
    void LoadData(const std::string& fileName);

    // Box2d
    class b2World* GetWorld() { return mWorld; }

    void Load() override;
private:
    class Game *mGame;
    Transform *tf;
    b2World *mWorld;
    class Player *mFireboy;
    class Player *mWatergirl;
    std::vector<b2Body*> mWorldColliders;
    std::vector<std::vector<Vector2>> mRamps;

    // Tiled
    std::string mLayerFileName;
    std::string mObjectsFileName;

    class MyContactListener *mContactListener;
    std::vector<WorldBodyComponent*> mBodies;
    std::vector<class Actor*> mActors;
    std::vector<class Platform*> mPlatforms;
};

#endif
