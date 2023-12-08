//
// Created by guilh on 07/12/2023.
//

#ifndef FOGO_E_AGUA_SENSORBODYCOMPONENT_H
#define FOGO_E_AGUA_SENSORBODYCOMPONENT_H

#include "WorldBodyComponent.h"
#include <iostream>
#include "../CSV.h"
#include <Box2D/Box2D.h>
#include <SDL.h>
#include <string>
#include "../Transform.h"
#include <iostream>
#include "../Actors/Actor.h"

class SensorBodyComponent : public WorldBodyComponent {
public:
    SensorBodyComponent(const std::string type, const std::string &affectBody, const std::string &line, b2World* world, Transform* transform, Actor* owner, float runVelocity = 8, float jumpVelocity = 12);
    const std::string &GetAffectBody() const { return mAffectBody; }
    const std::string &GetFunction() const { return mFunction; }
private:
    std::string mFunction;
    std::string mAffectBody;
};


#endif
