#include "Player.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Player::Player(Game* game, b2Body* body, PlayerType type,
           const float forwardSpeed,
           const float jumpSpeed)
        : Actor(game)
        , mType(type)
        , mIsRunning(false)
        , mIsJumping(false)
        , mWinner(false)
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

    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Characters/CharAssets.png", "../Assets/Sprites/Characters/CharAssets.json");

    if (mType == PlayerType::FireBoyHead)
    {
        mDrawComponent->AddAnimation("Idle", std::vector<int>{56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74});
        mDrawComponent->AddAnimation("Running", std::vector<int>{75,76,77,78,79,80,81,82,83,84,85});
        mDrawComponent->AddAnimation("Jumping", std::vector<int>{86,87,88,89,90});
        mDrawComponent->AddAnimation("Falling", std::vector<int>{51,52,53,54,55});
        mDrawComponent->AddAnimation("Stairs", std::vector<int>{103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132});
    }
    else if (mType == PlayerType::FireBoyLegs)
    {
        mDrawComponent->AddAnimation("Idle", std::vector<int>{94});
        mDrawComponent->AddAnimation("Running", std::vector<int>{95,96,97,98,99,100,101,102});
    }
    else if (mType == PlayerType::WaterGirlHead)
    {
        mDrawComponent->AddAnimation("Idle", std::vector<int>{145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174});
        mDrawComponent->AddAnimation("Running", std::vector<int>{175,176,177,178,179,180,181,182,183,184,185});
        mDrawComponent->AddAnimation("Jumping", std::vector<int>{186,187,188,189,190,191,192,193,194,195,196});
        mDrawComponent->AddAnimation("Falling", std::vector<int>{134,135,136,137,138,139,140,141,142,143,144});
        mDrawComponent->AddAnimation("Stairs", std::vector<int>{209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238});
    }
    else if (mType == PlayerType::WaterGirlLegs)
    {
        mDrawComponent->AddAnimation("Idle", std::vector<int>{200});
        mDrawComponent->AddAnimation("Running", std::vector<int>{201,202,203,204,205,206,207,208});
    }

    mDrawComponent->SetAnimation("Idle");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Player::OnProcessInput(const uint8_t* state)
{
    if(state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
    {
        mPlayerBody->ApplyLinearImpulseToCenter(b2Vec2(mForwardSpeed, 0),true);
        mRotation = 0;
        mIsRunning = true;
    }
    else if(state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
    {
        mPlayerBody->ApplyLinearImpulseToCenter(b2Vec2((-1)*mForwardSpeed, 0),true);
        mRotation = Math::Pi;
        mIsRunning = true;
    }
    else
    {
        mIsRunning = false;
    }

    if(state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
    {
        mPlayerBody->ApplyLinearImpulseToCenter(b2Vec2(0, mJumpSpeed),true);
        mIsOnGround = false;
    }
}

void Player::OnUpdate(float deltaTime)
{
    if(mPosition.x < GetGame()->GetCameraPos().x)
        mPosition.x = GetGame()->GetCameraPos().x;

    if(mPosition.y > (float)GetGame()->GetWindowHeight())
        Kill();

    ManageAnimations();
}

void Player::ManageAnimations()
{
}

void Player::Kill()
{
}

void Player::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses)
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
