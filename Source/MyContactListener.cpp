//
// Created by guilh on 06/12/2023.
//

#include "MyContactListener.h"

void MyContactListener::BeginContact(b2Contact* contact) {
    auto* bodyA = &contact->GetFixtureA()->GetBody()->GetUserData();
    auto* bodyB = &contact->GetFixtureB()->GetBody()->GetUserData();
    auto a = reinterpret_cast<WorldBodyComponent*>(bodyA->pointer);
    auto b = (WorldBodyComponent*)(bodyB->pointer);
    if(a && b) {
        if(isPlayerOnGround(a, b)) {
            if(a->GetType() == BodyTypes::Player) a->SetIsOnGround(true);
            else b->SetIsOnGround(true);
        }
    }
}

void MyContactListener::EndContact(b2Contact *contact) {
    auto* bodyA = &contact->GetFixtureA()->GetBody()->GetUserData();
    auto* bodyB = &contact->GetFixtureB()->GetBody()->GetUserData();
    auto a = reinterpret_cast<WorldBodyComponent*>(bodyA->pointer);
    auto b = (WorldBodyComponent*)(bodyB->pointer);
    if(a && b) {
        if(isPlayerOnGround(a, b)) {
            if(a->GetType() == BodyTypes::Player) a->SetIsOnGround(false);
            else b->SetIsOnGround(false);
        }
    }
}

bool MyContactListener::isPlayerOnGround(WorldBodyComponent *a, WorldBodyComponent *b) {
    if(a->GetType() == BodyTypes::Player || b->GetType() == BodyTypes::Player)
        if(a->GetType() == BodyTypes::Floor || b->GetType() == BodyTypes::Floor)
            return true;
    return false;
}