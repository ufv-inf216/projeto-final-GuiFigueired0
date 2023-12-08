#pragma once
#include "Actor.h"
#include <Box2D/Box2D.h>
#include <string>
#include "../Transform.h"
#include "../Components/SensorBodyComponent.h"

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
    explicit Liquid(const std::string &type, const std::string &affectBody, const std::string &orientation, Game* game, const std::string &line, b2World* world, Transform* transform);

    SensorBodyComponent* GetBodyComponent() { return mSensorBodyComponent; }

    LiquidType GetType() { return mType; }

private:
    LiquidType mType;
    LiquidPos mPos;

    class DrawAnimatedComponent* mDrawComponent;
    SensorBodyComponent* mSensorBodyComponent;
    b2Body *mPlayerBody;
};
