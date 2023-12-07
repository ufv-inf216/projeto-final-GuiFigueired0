//
// Created by guilh on 01/12/2023.
//

#include "WorldBodyComponent.h"
#include <iostream>
#include "../CSV.h"

WorldBodyComponent::WorldBodyComponent(const std::string &line, b2World* world, Transform* transform, float runVelocity, float jumpVelocity)
    :   tf(transform), mRunVelocity(runVelocity), mJumpVelocity(jumpVelocity), mIsOnGround(false), mWorld(world)
{
    auto tiles = CSVHelper::Split(line);
    float x = std::stof(tiles[2]);
    float y = std::stof(tiles[3]);
    Vector2 pos(x, y);

    mClass = tiles[0];
    if(tiles[1] == "WaterGirl" || tiles[1] == "FireBoy") mType = BodyTypes::Player;
    if(tiles[1] == "Wall") mType = BodyTypes::Wall;
    if(tiles[1] == "Floor" || tiles[1] == "Block") mType = BodyTypes::Floor;
    if(tiles[1] == "Ceiling") mType = BodyTypes::Ceiling;
    if(tiles[1] == "Ball") mType = BodyTypes::Ball;
    if(tiles[1] == "Portal") mType = BodyTypes::Sensor;

    if(tiles[0] == "Ramp") {
        // Defina os vértices para um triângulo (em metros)
        b2Vec2 vertices[3];
        vertices[0] = tf->pointMapToWorld(std::stof(tiles[4])+x, std::stof(tiles[5])+y);
        vertices[1] = tf->pointMapToWorld(std::stof(tiles[6])+x, std::stof(tiles[7])+y);
        vertices[2] = tf->pointMapToWorld(std::stof(tiles[8])+x, std::stof(tiles[9])+y);

        b2BodyDef rampBodyDef;
        mBody = GetWorld()->CreateBody(&rampBodyDef);
        b2PolygonShape shape;
        shape.Set(vertices, 3);
        mBody->CreateFixture(&shape, 1.0f);

        auto fixtureDef = new b2FixtureDef();
        fixtureDef->shape = &shape;
        fixtureDef->density = 1.0f;
        fixtureDef->friction = 0.0f;
        fixtureDef->restitution = 0.0f;
        fixtureDef->filter.categoryBits = BodyTypes::Floor;
        fixtureDef->filter.maskBits = BodyTypes::Player;
        mBody->CreateFixture(fixtureDef);
        fixtureDef->userData.pointer = reinterpret_cast<uintptr_t>(this);
    } else {
        float width = std::stof(tiles[4]);
        float height = std::stof(tiles[5]);
        Vector2 size(width, height);

        if(tiles[0] == "Floor" || tiles[0] == "Box")
        {
            mBody = CreateBody(pos, size, false, BodyTypes::Floor, BodyTypes::Player);
            //mWorldColliders.push_back(body);
        }
        else if(tiles[0] == "Player")
        {
            mBody = CreateBody(pos, size, true, BodyTypes::Player, BodyTypes::Floor);
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

class b2Body *WorldBodyComponent::CreateBody(const Vector2 &position, const Vector2 &size, bool isDynamic, BodyTypes type, BodyTypes collidesWith, bool fixedRotation) {
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

    auto fixtureDef = new b2FixtureDef();
    fixtureDef->shape = &shape;
    fixtureDef->density = 1.0f;
    fixtureDef->friction = 0.0f;
    fixtureDef->restitution = 0.0f;
    fixtureDef->filter.categoryBits = type;
    fixtureDef->filter.maskBits = collidesWith;
    fixtureDef->userData.pointer = (uintptr_t)(this);
    body->CreateFixture(fixtureDef);
    return body;
}

void WorldBodyComponent::Jump(){
    mBody->SetLinearVelocity(b2Vec2(mBody->GetLinearVelocity().x, mJumpVelocity));
}

void WorldBodyComponent::Run(bool toTheRight){
    mBody->SetLinearVelocity(b2Vec2(mRunVelocity*((toTheRight)?1.0f:-1.0f), mBody->GetLinearVelocity().y));
}