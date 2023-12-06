//
// Created by guilh on 01/12/2023.
//

#include "WorldBodyComponent.h"
#include <iostream>
#include "../CSV.h"

WorldBodyComponent::WorldBodyComponent(const std::string &line, b2World* world, Transform* transform, float runVelocity, float jumpVelocity)
    :   tf(transform), mRunVelocity(runVelocity), mJumpVelocity(jumpVelocity), mIsJumping(false), mWorld(world)
{
    auto tiles = CSVHelper::Split(line);
    float x = std::stof(tiles[1]);
    float y = std::stof(tiles[2]);
    Vector2 pos(x, y);

    mClass = tiles[0];
    if(tiles[0] == "Ramp") {
        // Defina os vértices para um triângulo (em metros)
        b2Vec2 vertices[3];
        vertices[0] = tf->pointMapToWorld(std::stof(tiles[3])+x, std::stof(tiles[4])+y);
        vertices[1] = tf->pointMapToWorld(std::stof(tiles[5])+x, std::stof(tiles[6])+y);
        vertices[2] = tf->pointMapToWorld(std::stof(tiles[7])+x, std::stof(tiles[8])+y);

        b2BodyDef rampBodyDef;
        mBody = GetWorld()->CreateBody(&rampBodyDef);
        b2PolygonShape shape;
        shape.Set(vertices, 3);
        mBody->CreateFixture(&shape, 1.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.filter.categoryBits = BodyTypes::Floor;
        fixtureDef.filter.maskBits = BodyTypes::Player;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
        mBody->CreateFixture(&fixtureDef);


    } else {
        mType = tiles[5];
        float width = std::stof(tiles[3]);
        float height = std::stof(tiles[4]);
        Vector2 size(width, height);

        if(tiles[0] == "Floor" || tiles[0] == "Box")
        {
            mBody = CreateBody(pos, size, false, BodyTypes::Floor, BodyTypes::Player);
            //mWorldColliders.push_back(body);
        }
        else if(tiles[0] == "Player")
        {
            mBody = CreateBody(pos, size, true, BodyTypes::Player, BodyTypes::Floor);
            b2Vec2 worldSize(tf->sizeMapToWorld(size.x), tf->sizeMapToWorld(size.y));
            b2Vec2 worldPos = tf->posMapToWorld(pos, size);
        }
    }
    Update();
}

void WorldBodyComponent::Update(){
    if(GetClass()=="Ramp" || GetClass() == "Sensor" || GetClass() == "Ball") return;
    auto shape = dynamic_cast<b2PolygonShape*>(mBody->GetFixtureList()->GetShape());
    b2Vec2 worldSize = shape->m_vertices[2] - shape->m_vertices[0];

    mMapValues.size = Vector2(worldSize.x*32, worldSize.y*32);
    mMapValues.pos = tf->posWorldToMap(mBody->GetPosition(), worldSize);
    mMapValues.angle = mBody->GetAngle();
    mMapValues.velocity = Vector2(mBody->GetLinearVelocity().x, mBody->GetLinearVelocity().y);
    mBody->SetLinearVelocity(b2Vec2(0, mBody->GetLinearVelocity().y));
    //std::cout << "Velocity: " << mBody->GetLinearVelocity().x << ", " << mBody->GetLinearVelocity().y << std::endl;
}

class b2Body *WorldBodyComponent::CreateBody(const Vector2 &position, const Vector2 &size, bool isDynamic, short type, short collidesWith, bool fixedRotation) {
    b2BodyDef bodyDef;
    if(isDynamic)
        bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = fixedRotation;
    b2Vec2 worldPos = tf->posMapToWorld(position, size);
    bodyDef.position = worldPos;
    b2Body* body = GetWorld()->CreateBody(&bodyDef);

    b2PolygonShape shape;
    b2Vec2 worldSize(tf->sizeMapToWorld(size.x), tf->sizeMapToWorld(size.y));
    shape.SetAsBox(worldSize.x, worldSize.y);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = type;
    fixtureDef.filter.maskBits = collidesWith;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    body->CreateFixture(&fixtureDef);
    return body;
}

void WorldBodyComponent::Jump(){
    mBody->SetLinearVelocity(b2Vec2(mBody->GetLinearVelocity().x, mJumpVelocity));
}

void WorldBodyComponent::Run(bool toTheRight){
    mBody->SetLinearVelocity(b2Vec2(mRunVelocity*((toTheRight)?1.0f:-1.0f), mBody->GetLinearVelocity().y));
}