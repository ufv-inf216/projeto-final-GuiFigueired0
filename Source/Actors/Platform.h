//
// Created by guilh on 14/12/2023.
//

#ifndef FOGO_E_AGUA_PLATFORM_H
#define FOGO_E_AGUA_PLATFORM_H

#include <Box2D/Box2D.h>
#include "Actor.h"
#include "../CSV.h"
#include "../Game.h"
#include "../Transform.h"
#include "../Components/SensorBodyComponent.h"
#include "../Components/WorldBodyComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include <string>
#include <vector>

class Platform : public Actor {
public:
    Platform(Game* game, const std::string &line, b2World* world, Transform* transform);

    WorldBodyComponent* GetBodyComponent() { return mWorldBodyComponent; }
    int GetId() { return id; }

    void SetColision();

    void OnUpdate();
private:
    class WorldBodyComponent* mWorldBodyComponent;
    class Transform* tf;
    int id;
    float mVelocity;
    float mTopLimit;
    float mBottomLimit;
};


#endif
