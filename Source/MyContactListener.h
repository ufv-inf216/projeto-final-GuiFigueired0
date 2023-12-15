//
// Created by guilh on 06/12/2023.
//

#ifndef FOGO_E_AGUA_MYCONTACTLISTENER_H
#define FOGO_E_AGUA_MYCONTACTLISTENER_H

#include <SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include "Math.h"
#include "CSV.h"
#include "Scenes/Level.h"
#include <Box2D/Box2D.h>
#include "Transform.h"
#include "Actors/Actor.h"
#include "Actors/Player.h"
#include "Actors/Platform.h"
#include "Game.h"
#include "Actors/Player.h"
#include "Components/WorldBodyComponent.h"
#include "Components/SensorBodyComponent.h"

class MyContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

    bool isPlayerOnGround(WorldBodyComponent* a, WorldBodyComponent* b);
    bool isPlayerOnBox(WorldBodyComponent* a, WorldBodyComponent* b);
    bool isPlayerOnSensor(WorldBodyComponent* a, WorldBodyComponent* b);
    bool isPlayerOnPlatform(WorldBodyComponent* a, WorldBodyComponent* b);
};


#endif
