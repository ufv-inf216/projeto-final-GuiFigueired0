#include "Liquid.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Liquid::Liquid(const std::string &type, const std::string &affectBody, const std::string &orientation, Game* game, const std::string &line, b2World* world, Transform* transform)
        : Actor(game)
{
    mSensorBodyComponent = new SensorBodyComponent(type, affectBody, line, world, transform, this);
    mPlayerBody = mSensorBodyComponent->GetBody();

    mType = affectBody == "WaterGirl" ? LiquidType::Lava : affectBody == "FireBoy" ? LiquidType::Water : LiquidType::Poison;
    mPos = orientation == "Right" ? LiquidPos::Right : orientation == "Left" ? LiquidPos::Left : LiquidPos::Center;

    Vector2 position;
    if((mType == LiquidType::Lava || mType == LiquidType::Poison) && mPos == LiquidPos::Center)
        position = Vector2(0,11);
    else if((mType == LiquidType::Lava || mType == LiquidType::Poison) && mPos == LiquidPos::Left)
        position = Vector2(0,7);
    else
        position = Vector2(0,8);

    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Ground/GroundAssets.png", "../Assets/Sprites/Ground/GroundAssets.json", 1, position, 150);

    if (mType == LiquidType::Lava)
    {
        if(mPos == LiquidPos::Right)
            mDrawComponent->AddAnimation("Idle", std::vector<int>{197,198,199,200,201,202,203,204,205,206,207,208,209,210,211});
        if(mPos == LiquidPos::Center)
            mDrawComponent->AddAnimation("Idle", std::vector<int>{167,168,169,170,171,172,173,174,175,176,177,178,179,180,181});
        if(mPos == LiquidPos::Left)
            mDrawComponent->AddAnimation("Idle", std::vector<int>{182,183,184,185,186,187,188,189,190,191,192,193,194,195,196});
    }
    else if (mType == LiquidType::Poison)
    {
        if(mPos == LiquidPos::Right)
            mDrawComponent->AddAnimation("Idle", std::vector<int>{243,244,245,246,247,248,249,250,251,252,253,254,255,256,257});
        if(mPos == LiquidPos::Center)
            mDrawComponent->AddAnimation("Idle", std::vector<int>{213,214,215,216,217,218,219,220,221,222,223,224,225,226,227});
        if(mPos == LiquidPos::Left)
            mDrawComponent->AddAnimation("Idle", std::vector<int>{228,229,230,231,232,233,234,235,236,237,238,239,240,241,242});
    }
    else if (mType == LiquidType::Water)
    {
        if(mPos == LiquidPos::Right)
            mDrawComponent->AddAnimation("Idle", std::vector<int>{307,308,309,310,311,312,313,314,315,316,317,318,319,320,321});
        if(mPos == LiquidPos::Center)
            mDrawComponent->AddAnimation("Idle", std::vector<int>{277,278,279,280,281,282,283,284,285,286,287,288,289,290,291});
        if(mPos == LiquidPos::Left)
            mDrawComponent->AddAnimation("Idle", std::vector<int>{292,293,294,295,296,297,298,299,300,301,302,303,304,305,306});
    }

    mDrawComponent->SetAnimation("Idle");
    mDrawComponent->SetAnimFPS(10.0f);
}
