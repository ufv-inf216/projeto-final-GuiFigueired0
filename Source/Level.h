//
// Created by guilh on 05/12/2023.
//

#ifndef FOGO_E_AGUA_LEVEL_H
#define FOGO_E_AGUA_LEVEL_H

#include <SDL.h>
#include <vector>
#include <string>
#include "Math.h"
#include "CSV.h"
#include <Box2D/Box2D.h>
#include "Transform.h"
#include "Game.h"
#include "Actors/Player.h"
#include <fstream>

class BodyTypes {
public:
    static short Player;
    static short Wall;
    static short Floor;
};

class Level {
public:
    Level(Game* game, std::string layerFileName, std::string objectsFileName);

    void InicializeLevel();
    void UpdateLevel(float deltaTime);
    void DrawColliders(SDL_Renderer *renderer);
    class Game* GetGame() { return mGame; }

    // Tiled
    void LoadData(const std::string& fileName);

    // Box2d
    class b2World* GetWorld() { return mWorld; }
    class b2Body* CreateBody(const Vector2& position, const Vector2 &size, bool isDynamic, short type, short collidesWith, bool fixedRotation = false);
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
};


#endif
