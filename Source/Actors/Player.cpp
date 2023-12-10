#include "Player.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../Math.h"
#include "../AudioSystem.h"

#define EPS 1e-9

Player::Player(Game* game, const std::string &line, b2World* world, PlayerType type, Transform* transform,
           const float forwardSpeed,
           const float jumpSpeed)
        : Actor(game)
        , mType(type)
        , mWinner(false)
        , mIsDead(false)
        , mIsRunning(false)
        , mForwardSpeed(forwardSpeed)
        , mJumpSpeed(jumpSpeed)
        , mWinnerTime(0)
{
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 32, 32, ColliderLayer::Player);
    mWorldBodyComponent = new WorldBodyComponent(line, world, transform, this);
    mPlayerBody = mWorldBodyComponent->GetBody();
    std::vector<Vector2> vertices;
    vertices.push_back(mColliderComponent->GetMin());
    vertices.push_back(mColliderComponent->GetMin() + Vector2(0, 32));
    vertices.push_back(mColliderComponent->GetMin() + Vector2(32, 0));
    vertices.push_back(mColliderComponent->GetMax());
    //new DrawPolygonComponent(this, vertices);

    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Characters/CharAssets.png", "../Assets/Sprites/Characters/CharAssets.json", 1/1.5f, Vector2(26,30), 110);

    if (mType == PlayerType::FireBoyHead)
    {
        mDrawComponent->AddAnimation("Idle", std::vector<int>{56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74});
        mDrawComponent->AddAnimation("Running", std::vector<int>{75,76,77,78,79,80,81,82,83,84,85});
        mDrawComponent->AddAnimation("Jumping", std::vector<int>{86,87,88,89,90});
        mDrawComponent->AddAnimation("Falling", std::vector<int>{51,52,53,54,55});
        mDrawComponent->AddAnimation("Stairs", std::vector<int>{103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132});
        mDrawComponent->AddAnimation("Dead", std::vector<int>{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45});
    }
    else if (mType == PlayerType::FireBoyLegs)
    {
        mDrawComponent->AddAnimation("Idle", std::vector<int>{94});
        mDrawComponent->AddAnimation("Running", std::vector<int>{95,96,97,98,99,100,101,102});
        mDrawComponent->AddAnimation("Default", std::vector<int>{0});
    }
    else if (mType == PlayerType::WaterGirlHead)
    {
        mDrawComponent->AddAnimation("Idle", std::vector<int>{145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174});
        mDrawComponent->AddAnimation("Running", std::vector<int>{175,176,177,178,179,180,181,182,183,184,185});
        mDrawComponent->AddAnimation("Jumping", std::vector<int>{186,187,188,189,190,191,192,193,194,195,196});
        mDrawComponent->AddAnimation("Falling", std::vector<int>{134,135,136,137,138,139,140,141,142,143,144});
        mDrawComponent->AddAnimation("Stairs", std::vector<int>{209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238});
        mDrawComponent->AddAnimation("Dead", std::vector<int>{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45});
    }
    else if (mType == PlayerType::WaterGirlLegs)
    {
        mDrawComponent->AddAnimation("Idle", std::vector<int>{200});
        mDrawComponent->AddAnimation("Running", std::vector<int>{201,202,203,204,205,206,207,208});
        mDrawComponent->AddAnimation("Default", std::vector<int>{0});
    }

    mDrawComponent->SetAnimation("Idle");
    mDrawComponent->SetAnimFPS(20.0f);
}

void Player::OnProcessInput(const uint8_t* state)
{
    if(!mIsDead && !mWinner) {
        if ((state[SDL_SCANCODE_UP] && (mType == PlayerType::FireBoyHead || mType == PlayerType::FireBoyLegs)) ||
            (state[SDL_SCANCODE_W] && (mType == PlayerType::WaterGirlHead || mType == PlayerType::WaterGirlLegs))) {
            if (GetBodyComponent()->IsOnGround()) {
                mWorldBodyComponent->Jump();
                if (mType == PlayerType::FireBoyHead)
                    if (GetGame()->GetSound()->GetSoundState(mSoundJump) != SoundState::Playing)
                        mSoundJump = GetGame()->GetSound()->PlaySound("Jump fb.mp3");
                if (mType == PlayerType::WaterGirlHead)
                    if (GetGame()->GetSound()->GetSoundState(mSoundJump) != SoundState::Playing)
                        mSoundJump = GetGame()->GetSound()->PlaySound("Jump wg.mp3");
            }
        }

        if (state[SDL_SCANCODE_RIGHT] && (mType == PlayerType::FireBoyHead || mType == PlayerType::FireBoyLegs) ||
            state[SDL_SCANCODE_D] && (mType == PlayerType::WaterGirlHead || mType == PlayerType::WaterGirlLegs)) {
            mWorldBodyComponent->Run(true);
            if (mWorldBodyComponent->GetVelocity().x != 0)
                mIsRunning = true;
            mRotation = 0;
        } else if (state[SDL_SCANCODE_LEFT] && (mType == PlayerType::FireBoyHead || mType == PlayerType::FireBoyLegs) ||
                   state[SDL_SCANCODE_A] &&
                   (mType == PlayerType::WaterGirlHead || mType == PlayerType::WaterGirlLegs)) {
            mWorldBodyComponent->Run(false);
            if (mWorldBodyComponent->GetVelocity().x != 0)
                mIsRunning = true;
            mRotation = Math::Pi;
        } else {
            mIsRunning = false;
        }

        if (state[SDL_SCANCODE_C] && mType == PlayerType::FireBoyHead) {
            GetGame()->changeShowColliders();
        }
    }
}

void Player::OnUpdate(float deltaTime)
{
    mWinner = GetGame()->GetStateWin();

    ManageAnimations();

    if(mWinner){
        mWinnerTime += deltaTime;
        return;
    }

    auto contactEdge = GetBodyComponent()->GetBody()->GetContactList();

    if (mType == PlayerType::WaterGirlHead)
        GetGame()->SetWinWaterGirl(false);
    if (mType == PlayerType::FireBoyHead)
        GetGame()->SetWinFireBoy(false);

    while (contactEdge != nullptr) {
        auto contact = contactEdge->contact;

        auto *bodyA = &contact->GetFixtureA()->GetBody()->GetUserData();
        auto *bodyB = &contact->GetFixtureB()->GetBody()->GetUserData();
        auto a = reinterpret_cast<WorldBodyComponent *>(bodyA->pointer);
        auto b = (WorldBodyComponent *) (bodyB->pointer);

        if (a->GetClass() == "Sensor" || b->GetClass() == "Sensor") {
            SensorBodyComponent *sensor;
            if (a->GetClass() == "Sensor")
                sensor = (SensorBodyComponent *) a;
            else
                sensor = (SensorBodyComponent *) b;

            if (mType == PlayerType::FireBoyHead && sensor->GetAffectBody() == "FireBoy" &&
                sensor->GetFunction() == "Portal" &&
                abs((GetPosition().x + mWorldBodyComponent->GetSize().x)/2 - (sensor->GetPosition().x + sensor->GetSize().x)/2) < 10)
                GetGame()->SetWinFireBoy(true);
            if (mType == PlayerType::WaterGirlHead && sensor->GetAffectBody() == "WaterGirl" &&
                sensor->GetFunction() == "Portal" &&
                abs((GetPosition().x + mWorldBodyComponent->GetSize().x)/2 - (sensor->GetPosition().x + sensor->GetSize().x)/2) < 10)
                GetGame()->SetWinWaterGirl(true);

            if((mType == PlayerType::FireBoyHead && sensor->GetAffectBody() == "FireBoy" ||
                mType == PlayerType::WaterGirlHead && sensor->GetAffectBody() == "WaterGirl")
                && sensor->GetFunction() == "Diamond" && sensor->GetOwner()->GetState() != ActorState::Invisible) {
                mGame->GetSound()->PlaySound("Diamond.mp3");
                sensor->GetOwner()->SetState(ActorState::Invisible);
            }
        }

        contactEdge = contactEdge->next;
    }
}

void Player::ManageAnimations()
{
    if(mWinner)
    {
        if(mWinnerTime >= 9*0.16) {
            if (mType == PlayerType::FireBoyHead || mType == PlayerType::WaterGirlHead)
                mDrawComponent->SetAnimation("Stairs");
            else
                mDrawComponent->SetAnimation("Default");
        }
        else{
            mDrawComponent->SetAnimation("Idle");
        }
    }
    else if(mIsDead)
    {
        if(mType == PlayerType::FireBoyHead || mType == PlayerType::WaterGirlHead)
            mDrawComponent->SetAnimation("Dead");
        else
            mDrawComponent->SetAnimation("Default");
    }
    else if(!mIsRunning)
    {
        if(GetBodyComponent()->IsOnGround())
        {
            mDrawComponent->SetAnimation("Idle");
        }
        else if(mWorldBodyComponent->GetVelocity().y < 0)
        {
            if(mType == PlayerType::FireBoyHead || mType == PlayerType::WaterGirlHead)
                mDrawComponent->SetAnimation("Falling");
            else
                mDrawComponent->SetAnimation("Idle");
        }
        else
        {
            if (mType == PlayerType::FireBoyHead || mType == PlayerType::WaterGirlHead)
                mDrawComponent->SetAnimation("Jumping");
            else
                mDrawComponent->SetAnimation("Idle");
        }
    }
    else
    {
        //AJUSTAR ANGULAÇÃO DA CABEÇA
        //SetAngle(mWorldBodyComponent->GetAngle()*(180.0f / M_PI));
        //std::cout << GetAngle() << '\n';
        mDrawComponent->SetAnimation("Running");
    }
}

void Player::Kill()
{
    GetGame()->GetSound()->PlaySound("Death.mp3");
    mIsDead = true;
}

void Player::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses)
{
    for(auto [side, overlap]: responses){
        if(overlap.target->GetLayer() == ColliderLayer::Blocks && side == CollisionSide::Down){
        } else if(overlap.target->GetLayer() == ColliderLayer::Enemy && side == CollisionSide::Down){
            overlap.target->GetOwner()->Kill();
            mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed/1.5f));
        } else if(overlap.target->GetLayer() == ColliderLayer::Enemy && (side == CollisionSide::Right || side == CollisionSide::Left)){
            if(GetBodyComponent()->IsOnGround()) {
                Kill();
            } else {
                overlap.target->GetOwner()->Kill();
                mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed/1.5f));
            }
        }

    }
}
