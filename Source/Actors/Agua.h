#pragma once
#include "Actor.h"
#include <Box2D/Box2D.h>

class Agua : public Actor
{
public:
    explicit Agua(Game* game, b2Body*,
                  float forwardSpeed = 1.0f,
                  float jumpSpeed = 1.5f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) override;

    void Kill() override;

private:
    void ManageAnimations();

    float mForwardSpeed;
    float mJumpSpeed;
    bool mIsRunning;
    bool mIsDead;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
    b2Body *mPlayerBody;
};