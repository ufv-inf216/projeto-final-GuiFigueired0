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
        /*
        auto* bodyA = &contact->GetFixtureA()->GetBody()->GetUserData();
        auto* bodyB = &contact->GetFixtureB()->GetBody()->GetUserData();
        if(bodyA && bodyB) {
            auto a = reinterpret_cast<WorldBodyComponent*>(bodyA->pointer);
            auto b = (WorldBodyComponent*)(bodyB->pointer);
            if(isPlayerOnGround(a, b)) {
                if(a->GetType() == BodyTypes::Player) {
                    a->SetIsOnGround(true);
                } else {
                    b->SetIsOnGround(true);
                }
                std::cout << "Is on ground\n";
            } else {
                std::cout << "Not on ground\n";
            }
        }
         */
    }
    void EndContact(b2Contact* contact) override {
        /*
        auto* bodyA = &contact->GetFixtureA()->GetUserData().pointer;
        auto* bodyB = &contact->GetFixtureB()->GetBody()->GetUserData();
        if(bodyA && bodyB) {
            auto* actorA = reinterpret_cast<WorldBodyComponent*>(bodyA);
            auto* actorB = reinterpret_cast<WorldBodyComponent*>(bodyB);
            if(isPlayerOnGround(actorA, actorB)) {
                if(actorA->GetType() == BodyTypes::Player) {
                    actorA->SetIsOnGround(false);
                } else {
                    actorB->SetIsOnGround(false);
                }
            }
        }
         */
    }

    bool isPlayerOnGround(WorldBodyComponent* a, WorldBodyComponent* b) {
        std::cout << "A: " << (int)a->GetType() << " B: " << (int)b->GetType() << std::endl;
        if(a->GetType() == BodyTypes::Player || b->GetType() == BodyTypes::Player) {
            if(a->GetType() == BodyTypes::Floor || b->GetType() == BodyTypes::Floor) {
                return true;
            }
        }
        return false;
    }
};


#endif
