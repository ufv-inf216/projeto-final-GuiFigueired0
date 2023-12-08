//
// Created by guilh on 01/12/2023.
//

#ifndef FOGO_E_AGUA_WORLDBODYCOMPONENT_H
#define FOGO_E_AGUA_WORLDBODYCOMPONENT_H

#include <Box2D/Box2D.h>
#include <SDL.h>
#include <string>
#include "../Transform.h"
#include <iostream>
#include "../Actors/Actor.h"

enum BodyTypes {
    Player = 1,
    Wall = 2,
    Floor = 4,
    Ceiling = 8,
    Box = 16,
    Ball = 32,
    Sensor = 64
};

class WorldBodyComponent {
public:
    WorldBodyComponent(const std::string &line, b2World* world, Transform* transform, Actor* owner = nullptr, float runVelocity = 8, float jumpVelocity = 12);
    ~WorldBodyComponent() = default;
    class b2Body* CreateBody(const Vector2& position, const Vector2 &size, bool isDynamic, BodyTypes type,
            BodyTypes collidesWith, bool fixedRotation = false, float density = 1.0f, float friction = 0.0f);

    Vector2 GetPosition();
    Vector2 GetSize();
    Vector2 GetVelocity();
    float GetAngle();
    class b2World* GetWorld() { return mWorld; }
    class b2Body* GetBody() { return mBody; }
    BodyTypes GetType() { return mType; }
    const std::string &GetClass() const { return mClass; }

    void Update();
    void Jump();
    void Run(bool toTheRight);
    bool IsOnGround() { return mIsOnGround; }
    void SetIsOnGround(bool isOnGround) { mIsOnGround = isOnGround; }
    int cont_Collision = 0;

    Actor* GetOwner() { return mOwner; }
    void SetOwner(Actor* owner) { mOwner = owner; }

private:
    b2Body* mBody;
    Transform* tf;
    b2World* mWorld;
    float mJumpVelocity;
    float mRunVelocity;
    bool mIsOnGround;
    BodyTypes mType;
    std::string mClass;
    Actor* mOwner;
};


#endif
