#include "Agua.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Agua::Agua(Game* game, b2Body* body,
           const float forwardSpeed,
           const float jumpSpeed)
        : Actor(game)
        , mIsRunning(false)
        , mIsDead(false)
        , mForwardSpeed(forwardSpeed)
        , mJumpSpeed(jumpSpeed)
        , mPlayerBody(body)
{
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 32, 32, ColliderLayer::Player);

    std::vector<Vector2> vertices;
    vertices.push_back(mColliderComponent->GetMin());
    vertices.push_back(mColliderComponent->GetMin() + Vector2(0, 32));
    vertices.push_back(mColliderComponent->GetMin() + Vector2(32, 0));
    vertices.push_back(mColliderComponent->GetMax());
    new DrawPolygonComponent(this, vertices);

    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Fogo/CharAssets.png", "../Assets/Sprites/Fogo/CharAssets.json");

    mDrawComponent->AddAnimation("Agua", std::vector<int>{145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174});

    mDrawComponent->SetAnimation("Agua");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Agua::OnProcessInput(const uint8_t* state)
{
    if(state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]){
        mPlayerBody->ApplyLinearImpulseToCenter(b2Vec2(mForwardSpeed, 0),true);
        mRotation = 0;
        mIsRunning = true;
    } else if(state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]){
        mPlayerBody->ApplyLinearImpulseToCenter(b2Vec2((-1)*mForwardSpeed, 0),true);
        mRotation = Math::Pi;
        mIsRunning = true;
    } else mIsRunning = false;
    // && mIsOnGround
    if(state[SDL_SCANCODE_SPACE] ) {
        mPlayerBody->ApplyLinearImpulseToCenter(b2Vec2(0, mJumpSpeed),true);

        mIsOnGround = false;
    }
}

void Agua::OnUpdate(float deltaTime)
{
    if(mPosition.x < GetGame()->GetCameraPos().x)
        mPosition.x = GetGame()->GetCameraPos().x;

    if(mPosition.y > (float)GetGame()->GetWindowHeight())
        Kill();

    ManageAnimations();
}

void Agua::ManageAnimations()
{

}

void Agua::Kill()
{
}

void Agua::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses)
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
