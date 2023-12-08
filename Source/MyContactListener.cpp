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
        if(isPlayerOnGround(a, b) || isPlayerOnBox(a, b)) {
            if(a->GetType() == BodyTypes::Player){
                a->SetIsOnGround(true);
                a->cont_Collision++;
            }
            else{
                b->SetIsOnGround(true);
                b->cont_Collision++;
            }
        } else if (isPlayerOnSensor(a, b)) {
            std::cout << "Begin Collision with portal\n";
                SensorBodyComponent* sensor;
                if(a->GetClass() == "Sensor")
                    sensor = (SensorBodyComponent*)a;
                else
                    sensor = (SensorBodyComponent*)b;
                std::cout << "Sensor: " << sensor->GetAffectBody() << "\n";
                std::cout << "Function: " << sensor->GetFunction() << "\n";

        }
    }
}

void MyContactListener::EndContact(b2Contact *contact) {
    auto* bodyA = &contact->GetFixtureA()->GetBody()->GetUserData();
    auto* bodyB = &contact->GetFixtureB()->GetBody()->GetUserData();
    auto a = reinterpret_cast<WorldBodyComponent*>(bodyA->pointer);
    auto b = (WorldBodyComponent*)(bodyB->pointer);
    if(a && b) {
        if(isPlayerOnGround(a, b) || isPlayerOnBox(a, b)) {
            if(a->GetType() == BodyTypes::Player){
                a->cont_Collision--;
                if(a->cont_Collision == 0)
                    a->SetIsOnGround(false);
            }
            else{
                b->cont_Collision--;
                if(b->cont_Collision == 0)
                    b->SetIsOnGround(false);
            }
        } else if (isPlayerOnSensor(a, b)) {
            std::cout << "End Collision with portal\n";
        }
    }
}

bool MyContactListener::isPlayerOnGround(WorldBodyComponent *a, WorldBodyComponent *b) {
    if(a->GetType() == BodyTypes::Player || b->GetType() == BodyTypes::Player)
        if(a->GetType() == BodyTypes::Floor || b->GetType() == BodyTypes::Floor)
            return true;
    return false;
}

bool MyContactListener::isPlayerOnBox(WorldBodyComponent *a, WorldBodyComponent *b) {
    if(a->GetType() == BodyTypes::Player || b->GetType() == BodyTypes::Player)
        if(a->GetClass() == "Block" || b->GetClass() == "Block")
            return true;
    return false;
}

bool MyContactListener::isPlayerOnSensor(WorldBodyComponent *a, WorldBodyComponent *b) {
    if(a->GetType() == BodyTypes::Player || b->GetType() == BodyTypes::Player)
        if(a->GetClass() == "Sensor" || b->GetClass() == "Sensor")
            return true;
    return false;
}