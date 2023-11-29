//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Spawner.h"
#include "../Game.h"
#include "Fogo.h"
#include "Goomba.h"

const float GOOMBA_FORWARD_SPEED = 100.0f;

Spawner::Spawner(Game* game, float spawnDistance)
        :Actor(game)
        ,mSpawnDistance(spawnDistance)
{

}

void Spawner::OnUpdate(float deltaTime)
{
    auto distance = GetGame()->GetFogo()->GetPosition() - GetPosition();
    if(distance.Length() < mSpawnDistance){
        auto goomba = new Goomba(GetGame(), GOOMBA_FORWARD_SPEED, 100.0f);
        goomba->SetPosition(GetPosition());
        goomba->GetComponent<RigidBodyComponent>()->SetVelocity(Vector2((-1)*GOOMBA_FORWARD_SPEED, 0));
        SetState(ActorState::Destroy);
    }
}