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
        mBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

        auto fixtureDef = new b2FixtureDef();
        fixtureDef->shape = &shape;
        fixtureDef->density = 1.0f;
        fixtureDef->friction = 0.0f;
        fixtureDef->restitution = 0.0f;
        fixtureDef->filter.categoryBits = BodyTypes::Floor;
        fixtureDef->filter.maskBits = BodyTypes::Player;
        mBody->CreateFixture(fixtureDef);
    } else {
        float width = std::stof(tiles[4]);
        float height = std::stof(tiles[5]);
        Vector2 size(width, height);

        if(tiles[1] == "Block")
        {
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position = tf->posMapToWorld(pos, size);
            b2Body* dynamicBody = GetWorld()->CreateBody(&bodyDef);

            // Adicionando uma forma ao corpo (um retângulo)
            b2PolygonShape dynamicBox;
            b2Vec2 worldSize(tf->sizeMapToWorld(size.x), tf->sizeMapToWorld(size.y));
            dynamicBox.SetAsBox(worldSize.x, worldSize.y);

            // Configurando a densidade, fricção e restituição
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 5.0f;
            fixtureDef.friction = 0.8f;
            fixtureDef.restitution = 0.5f;
            fixtureDef.filter.categoryBits = BodyTypes::Player | BodyTypes::Floor;
            dynamicBody->CreateFixture(&fixtureDef);
            mBody = dynamicBody;
        } else if(tiles[0] == "Floor" || tiles[0] == "Box")
        {
            mBody = CreateBody(pos, size, false, BodyTypes::Floor, BodyTypes::Player, true, 0.0f, 1.0f);
        }
        else if(tiles[0] == "Player")
        {
            mBody = CreateBody(pos, size, true, BodyTypes::Player, BodyTypes::Floor);
        }
    }
    Update();
}

class b2Body *WorldBodyComponent::CreateBody(const Vector2 &position, const Vector2 &size, bool isDynamic, BodyTypes type, BodyTypes collidesWith, bool fixedRotation, float density, float friction) {
    b2BodyDef bodyDef;
    if(isDynamic)
        bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = fixedRotation;
    b2Vec2 worldPos = tf->posMapToWorld(position, size);
    bodyDef.position = worldPos;
    b2Body* body = GetWorld()->CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    b2PolygonShape shape;
    b2Vec2 worldSize(tf->sizeMapToWorld(size.x), tf->sizeMapToWorld(size.y));
    shape.SetAsBox(worldSize.x, worldSize.y);

    auto fixtureDef = new b2FixtureDef();
    fixtureDef->shape = &shape;
    fixtureDef->density = density;
    fixtureDef->friction = friction;
    fixtureDef->restitution = 0.0f;
    fixtureDef->filter.categoryBits = type;
    fixtureDef->filter.maskBits = collidesWith;
    body->CreateFixture(fixtureDef);
    return body;
}

void WorldBodyComponent::Update(){
    if(GetClass()=="Ramp" || GetClass() == "Sensor" || GetClass() == "Ball") return;
    if(GetType() == BodyTypes::Player)
        mBody->SetLinearVelocity(b2Vec2(0, mBody->GetLinearVelocity().y));
}

Vector2 WorldBodyComponent::GetVelocity() {
    return Vector2(mBody->GetLinearVelocity().x, mBody->GetLinearVelocity().y);;
}

Vector2 WorldBodyComponent::GetPosition() {
    auto shape = dynamic_cast<b2PolygonShape*>(mBody->GetFixtureList()->GetShape());
    b2Vec2 worldSize = shape->m_vertices[2] - shape->m_vertices[0];
    return tf->posWorldToMap(mBody->GetPosition(), worldSize);
}

Vector2 WorldBodyComponent::GetSize() {
    auto shape = dynamic_cast<b2PolygonShape*>(mBody->GetFixtureList()->GetShape());
    b2Vec2 worldSize = shape->m_vertices[2] - shape->m_vertices[0];
    return Vector2(worldSize.x*32, worldSize.y*32);

}

float WorldBodyComponent::GetAngle() {
    return mBody->GetAngle();
}

void WorldBodyComponent::Jump(){
    mBody->SetLinearVelocity(b2Vec2(mBody->GetLinearVelocity().x, mJumpVelocity));
}

void WorldBodyComponent::Run(bool toTheRight){
    mBody->SetLinearVelocity(b2Vec2(mRunVelocity*((toTheRight)?1.0f:-1.0f), mBody->GetLinearVelocity().y));
}