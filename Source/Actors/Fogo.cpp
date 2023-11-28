//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Fogo.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Fogo::Fogo(Game* game,
             const float forwardSpeed,
             const float jumpSpeed)
        : Actor(game)
        , mIsRunning(false)
        , mIsDead(false)
        , mForwardSpeed(forwardSpeed)
        , mJumpSpeed(jumpSpeed)
{
    mRigidBodyComponent = new RigidBodyComponent(this, 1.25, 5);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 32, 32, ColliderLayer::Player);
    /*
    std::vector<Vector2> vertices;
    vertices.push_back(mColliderComponent->GetMin());
    vertices.push_back(mColliderComponent->GetMin() + Vector2(0, 32));
    vertices.push_back(mColliderComponent->GetMin() + Vector2(32, 0));
    vertices.push_back(mColliderComponent->GetMax());
    new DrawPolygonComponent(this, vertices);
    */
    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Fogo/Fogo.png", "../Assets/Sprites/Fogo/Fogo.json");

    mDrawComponent->AddAnimation("Fogo", std::vector<int>{0});

    mDrawComponent->SetAnimation("Fogo");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Fogo::OnProcessInput(const uint8_t* state)
{
    if(state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]){
        mRigidBodyComponent->ApplyForce(Vector2(mForwardSpeed, 0));
        mRotation = 0;
        mIsRunning = true;
    } else if(state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]){
        mRigidBodyComponent->ApplyForce(Vector2((-1)*mForwardSpeed, 0));
        mRotation = Math::Pi;
        mIsRunning = true;
    } else mIsRunning = false;

    if(state[SDL_SCANCODE_SPACE] && mIsOnGround) {
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed));
        mIsOnGround = false;
    }
}

void Fogo::OnUpdate(float deltaTime)
{
    if(mPosition.x < GetGame()->GetCameraPos().x)
        mPosition.x = GetGame()->GetCameraPos().x;

    if(mPosition.y > (float)GetGame()->GetWindowHeight())
        Kill();

    ManageAnimations();
}

void Fogo::ManageAnimations()
{

}

void Fogo::Kill()
{
}

void Fogo::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses)
{
    for(auto [side, overlap]: responses){
        if(overlap.target->GetLayer() == ColliderLayer::Blocks && side == CollisionSide::Down){
            mIsOnGround = true;
        } else if(overlap.target->GetLayer() == ColliderLayer::Enemy && side == CollisionSide::Down){
            overlap.target->GetOwner()->Kill();
            mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed/1.5f));
        } else if(overlap.target->GetLayer() == ColliderLayer::Enemy && (side == CollisionSide::Right || side == CollisionSide::Left)){
            if(mIsOnGround) {
                Kill();
            } else {
                overlap.target->GetOwner()->Kill();
                mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed/1.5f));
            }
        }

    }
}
