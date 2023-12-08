#pragma once
#include "Actor.h"
#include <Box2D/Box2D.h>
#include <string>
#include "../Components/WorldBodyComponent.h"
#include "../Transform.h"

enum class LiquidType
{
    Lava,
    Poison,
    Water
};

enum class LiquidPos
{
    Left,
    Center,
    Right
};

class Liquid : public Actor
{
public:
    explicit Liquid(Game* game, const std::string &line, b2World* world, LiquidType lType, LiquidPos lPos, Transform* transform);

    WorldBodyComponent* GetBodyComponent() { return mWorldBodyComponent; }

private:
    LiquidType mType;
    LiquidPos mPos;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
    WorldBodyComponent* mWorldBodyComponent;
    b2Body *mPlayerBody;
};
