//
// Created by guilh on 07/12/2023.
//

#include "SensorBodyComponent.h"

SensorBodyComponent::SensorBodyComponent(const std::string &type, const std::string &affectBody, const std::string &line,
                                         b2World *world, Transform *transform, Actor* owner, float runVelocity, float jumpVelocity)
                                         : WorldBodyComponent(line, world, transform, owner, runVelocity, jumpVelocity),
                                           mFunction(type), mAffectBody(affectBody) {}