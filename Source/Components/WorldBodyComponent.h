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

struct MapValues {
    Vector2 pos, size, velocity;
    float angle;
};


class BodyTypes {
public:
    static short Player;
    static short Wall;
    static short Floor;
};

class WorldBodyComponent {
public:
    WorldBodyComponent(const std::string &line, b2World* world, Transform* transform, float runVelocity = 8, float jumpVelocity = 12);
    ~WorldBodyComponent() = default;
    class b2Body* CreateBody(const Vector2& position, const Vector2 &size, bool isDynamic, short type, short collidesWith, bool fixedRotation = false);

    Vector2 GetPosition() { return mMapValues.pos; }
    Vector2 GetSize() { return mMapValues.size; }
    Vector2 GetVelocity() { return mMapValues.velocity; }
    float GetAngle() { return mMapValues.angle; }
    class b2World* GetWorld() { return mWorld; }
    class b2Body* GetBody() { return mBody; }
    const std::string &GetType() const { return mType; }
    const std::string &GetClass() const { return mClass; }

    void Update();
    void Jump();
    void Run(bool toTheRight);
    bool IsJumping() { return mIsJumping; }

    void hit() { std::cout << "I've been hit" << std::endl; }

private:
    b2Body* mBody;
    Transform* tf;
    b2World* mWorld;
    MapValues mMapValues;
    float mJumpVelocity;
    float mRunVelocity;
    bool mIsJumping;
    std::string mType;
    std::string mClass;
};


#endif
