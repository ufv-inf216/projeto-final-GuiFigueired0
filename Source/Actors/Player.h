#pragma once
#include "Actor.h"
#include <Box2D/Box2D.h>

enum class PlayerType
{
    FireBoyHead,
    FireBoyLegs,
    WaterGirlHead,
    WaterGirlLegs
};

class Player : public Actor
{
public:
    explicit Player(Game* game, b2Body*, PlayerType type,
                  float forwardSpeed = 1.0f,
                  float jumpSpeed = 1.5f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) override;

    void Kill() override;

private:
    void ManageAnimations();

    PlayerType mType;

    float mForwardSpeed;
    float mJumpSpeed;
    bool mIsRunning;
    bool mIsJumping;
    bool mIsDead;
    bool mWinner;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
    b2Body *mPlayerBody;
};