//
// Created by guilh on 01/12/2023.
//

#include "WorldBodyComponent.h"

WorldBodyComponent::WorldBodyComponent(b2Body* body, Transform* transform, float runVelocity, float jumpVelocity)
    : mBody(body), tf(transform), mRunVelocity(runVelocity), mJumpVelocity(jumpVelocity)
{
    Update();
}

void WorldBodyComponent::Update(){
    auto shape = dynamic_cast<b2PolygonShape*>(mBody->GetFixtureList()->GetShape());
    b2Vec2 worldSize = shape->m_vertices[2] - shape->m_vertices[0];

    mMapValues.size = Vector2(worldSize.x*32, worldSize.y*32);
    mMapValues.pos = tf->posWorldToMap(mBody->GetPosition(), worldSize);
    mMapValues.angle = mBody->GetAngle();
    mMapValues.velocity = Vector2(mBody->GetLinearVelocity().x, mBody->GetLinearVelocity().y);
    mBody->SetLinearVelocity(b2Vec2(0, mBody->GetLinearVelocity().y));
}

void WorldBodyComponent::Jump(){
    mBody->SetLinearVelocity(b2Vec2(mBody->GetLinearVelocity().x, mJumpVelocity));
}

void WorldBodyComponent::Run(bool toTheRight){
    mBody->SetLinearVelocity(b2Vec2(mRunVelocity*((toTheRight)?1.0f:-1.0f), mBody->GetLinearVelocity().y));
}