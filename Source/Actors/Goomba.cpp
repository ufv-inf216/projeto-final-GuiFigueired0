//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Goomba.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

Goomba::Goomba(Game* game, float forwardSpeed, float deathTime)
        : Actor(game)
        , mDyingTimer(deathTime)
        , mIsDying(false)
        , mForwardSpeed(forwardSpeed)
{
    mRigidBodyComponent = new RigidBodyComponent(this);
    mRigidBodyComponent->SetVelocity(Vector2( mForwardSpeed, 0));

    mColliderComponent = new AABBColliderComponent(this, 0, 0, 32, 32, ColliderLayer::Enemy);

    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Goomba/Goomba.png", "../Assets/Sprites/Goomba/Goomba.json");
    mDrawComponent->AddAnimation("dead", std::vector<int>{0});
    mDrawComponent->AddAnimation("walk", std::vector<int>{ 1,2 });
    mDrawComponent->SetAnimation("walk");
    mDrawComponent->SetAnimFPS(5.0f);
}

void Goomba::Kill()
{
    mDrawComponent->SetAnimation("dead");
    mIsDying = true;
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);
}

void Goomba::OnUpdate(float deltaTime)
{
    if(mIsDying) mDyingTimer -= deltaTime;
    if(mDyingTimer <= 0) SetState(ActorState::Destroy);
    if(mPosition.y > (float)GetGame()->GetWindowHeight())
        SetState(ActorState::Destroy);
}

void Goomba::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses)
{
    for(auto response: responses){
        if(response.first==CollisionSide::Left)
            mRigidBodyComponent->SetVelocity(Vector2(mForwardSpeed, mRigidBodyComponent->GetVelocity().y));
        else if(response.first==CollisionSide::Right)
            mRigidBodyComponent->SetVelocity(Vector2(mForwardSpeed*(-1), mRigidBodyComponent->GetVelocity().y));
    }
}