//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AABBColliderComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include <vector>
#include <algorithm>

AABBColliderComponent::AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h, ColliderLayer layer, int updateOrder)
        :Component(owner, updateOrder)
        ,mOffset(Vector2((float)dx, (float)dy))
        ,mWidth(w)
        ,mHeight(h)
        ,mLayer(layer)
{
    mOwner->GetGame()->AddCollider(this);
}

AABBColliderComponent::~AABBColliderComponent()
{
    mOwner->GetGame()->RemoveCollider(this);
}

Vector2 AABBColliderComponent::GetMin() const
{
    return GetOwner()->GetPosition() + mOffset;
}

Vector2 AABBColliderComponent::GetMax() const
{
    return GetMin() + Vector2((float)mWidth, (float)mHeight);
}

Vector2 AABBColliderComponent::GetCenter() const
{
    return GetMin() + Vector2((float)mWidth/2.0f, (float)mHeight/2.0f);
}

bool AABBColliderComponent::Intersect(const AABBColliderComponent& b) const
{
   bool noCollision = (GetMax().x < b.GetMin().x) || (b.GetMax().x < GetMin().x) ||
                        (GetMax().y < b.GetMin().y) || (b.GetMax().y < GetMin().y);
   return !noCollision;
}

AABBColliderComponent::Overlap AABBColliderComponent::GetMinOverlap(AABBColliderComponent* b) const
{
    //std::unordered_map<int, Overlap> overlaps;
    std::vector<float> overlaps;
    overlaps.emplace_back(b->GetMax().y - GetMin().y); // top
    overlaps.emplace_back(b->GetMin().y - GetMax().y); // bottom
    overlaps.emplace_back(b->GetMax().x - GetMin().x); // left
    overlaps.emplace_back(b->GetMin().x - GetMax().x); // right
    Overlap overlap;
    overlap.side = static_cast<CollisionSide>(0);
    overlap.amount = overlaps[0];
    overlap.target = b;
    for(int i=1; i<overlaps.size(); i++){
        if(std::fabs(overlaps[i]) < std::fabs(overlap.amount)){
            overlap.amount = overlaps[i];
            overlap.side = static_cast<CollisionSide>(i);
        }
    }
    return overlap;
}

void AABBColliderComponent::ResolveCollisions(RigidBodyComponent *rigidBody, const Overlap& minOverlap)
{
    SDL_Log("%d, %f", minOverlap.side, minOverlap.amount);
    if((minOverlap.side == CollisionSide::Top && rigidBody->GetVelocity().y < 0) ||
        (minOverlap.side == CollisionSide::Down && rigidBody->GetVelocity().y > 0)){
        GetOwner()->SetPosition(GetOwner()->GetPosition() + Vector2(0.0f, minOverlap.amount));
        rigidBody->SetVelocity(Vector2(rigidBody->GetVelocity().x, 0));
    } else if((minOverlap.side == CollisionSide::Left && rigidBody->GetVelocity().x < 0) ||
                (minOverlap.side == CollisionSide::Right && rigidBody->GetVelocity().x > 0)){
        GetOwner()->SetPosition(GetOwner()->GetPosition() + Vector2(minOverlap.amount, 0.0f));
        rigidBody->SetVelocity(Vector2(0, rigidBody->GetVelocity().y));
    }
}

void AABBColliderComponent::DetectCollision(RigidBodyComponent *rigidBody)
{
    // Sort colliders by amount to the player (center-to-center)
    auto colliders = mOwner->GetGame()->GetColliders();

    std::sort(colliders.begin(), colliders.end(),
              [this](AABBColliderComponent* a, AABBColliderComponent* b){
                  return Vector2(a->GetCenter() - GetCenter()).LengthSq() <= Vector2(b->GetCenter() - GetCenter()).LengthSq();
              });

    std::unordered_map<CollisionSide, Overlap> responses;

    std::pair<bool, bool> sides = { false, false }; // é pra verificar se ocorreu as colisoes vertical e horizontal
    for(auto collider: colliders){
        if(collider == this || !collider->IsEnabled()) continue;
        if(Intersect(*collider)){
            auto overlap = GetMinOverlap(collider);
            //SDL_Log("Houve Colisao %d %d", collider->GetLayer(), GetLayer());
            ResolveCollisions(rigidBody, overlap);
            responses[overlap.side] = overlap;                                           // colisao horizontal  , colisao vertical
            (overlap.side == CollisionSide::Left || overlap.side == CollisionSide::Right) ? sides.second = true : sides.first = true;
        }
        if(sides.first && sides.second) break;
    }

    // Callback only for closest (first) collision
    mOwner->OnCollision(responses);
}