//
// Created by guilh on 14/12/2023.
//

#ifndef FOGO_E_AGUA_PLATFORM_H
#define FOGO_E_AGUA_PLATFORM_H

#include <Box2D/Box2D.h>
#include "Actor.h"
#include "../CSV.h"
#include "../Game.h"
#include "../Transform.h"
#include "../Components/SensorBodyComponent.h"
#include "../Components/WorldBodyComponent.h"
#include <string>
#include <vector>

class Platform : public Actor {
public:
    Platform(Game* game, const std::string &line, b2World* world, Transform* transform, float velocity = 1.0f);

    WorldBodyComponent* GetBodyComponent() { return mWorldBodyComponent; }
    SensorBodyComponent* GetBottomSensor() { return bottomSensor; }
    SensorBodyComponent* GetTopSensor() { return topSensor; }
    int GetId() { return id; }

    void SetTopSensor(SensorBodyComponent* sensor) { topSensor = sensor; }
    void SetBottomSensor(SensorBodyComponent* sensor) { bottomSensor = sensor; }

    void OnUpdate();
private:
    class DrawAnimatedComponent* mDrawComponent;
    WorldBodyComponent* mWorldBodyComponent;
    SensorBodyComponent *topSensor, *bottomSensor;
    float mVelocity;
    int id;
    float hPos;
};


#endif
