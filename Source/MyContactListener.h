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
#include "Level.h"
#include <Box2D/Box2D.h>
#include "Transform.h"
#include "Actors/Actor.h"
#include "Actors/Player.h"
#include "Game.h"
#include "Actors/Player.h"
#include "Components/WorldBodyComponent.h"

class MyContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override {
        auto* bodyA = &contact->GetFixtureA()->GetBody()->GetUserData();
        auto* bodyB = &contact->GetFixtureB()->GetBody()->GetUserData();
        if(bodyA && bodyB) {
            auto* actorA = reinterpret_cast<WorldBodyComponent*>(bodyA);
            auto* actorB = reinterpret_cast<WorldBodyComponent*>(bodyB);
            actorA->hit();
        }
    }
};


#endif
