//
// Created by guilh on 01/12/2023.
//

#ifndef FOGO_E_AGUA_WORLDBODYCOMPONENT_H
#define FOGO_E_AGUA_WORLDBODYCOMPONENT_H

#include <Box2D/Box2D.h>
#include <SDL.h>
#include "../Transform.h"

struct MapValues {
    Vector2 pos, size, velocity;
    float angle;
};

class WorldBodyComponent {
public:
    WorldBodyComponent(b2Body* body, Transform* transform, float runVelocity = 8, float jumpVelocity = 8);
    ~WorldBodyComponent() = default;

    Vector2 GetPosition() { return mMapValues.pos; }
    Vector2 GetSize() { return mMapValues.size; }
    Vector2 GetVelocity() { return mMapValues.velocity; }
    float GetAngle() { return mMapValues.angle; }

    void Update();
    void Jump();
    void Run(bool toTheRight);
    bool IsJumping() { return mIsJumping; }

private:
    b2Body* mBody;
    Transform* tf;
    MapValues mMapValues;
    float mJumpVelocity;
    float mRunVelocity;
    bool mIsJumping;
};


#endif
