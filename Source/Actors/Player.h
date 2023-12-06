#pragma once
#include "Actor.h"
#include <Box2D/Box2D.h>
#include <string>
#include "../Components/WorldBodyComponent.h"
#include "../Transform.h"

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
    explicit Player(Game* game, const std::string &line, b2World* world, PlayerType type, Transform* transform,
                  float forwardSpeed = 1.0f,
                  float jumpSpeed = 1.5f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) override;

    void Kill() override;

    WorldBodyComponent* GetBodyComponent() { return mWorldBodyComponent; }

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
    WorldBodyComponent* mWorldBodyComponent;
    b2Body *mPlayerBody;
};