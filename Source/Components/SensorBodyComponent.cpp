//
// Created by guilh on 07/12/2023.
//

#include "SensorBodyComponent.h"

SensorBodyComponent::SensorBodyComponent(const std::string type, const std::string &affectBody, const std::string &line,
                                         b2World *world, Transform *transform, float runVelocity, float jumpVelocity)
                                         : WorldBodyComponent(line, world, transform, runVelocity, jumpVelocity),
                                           mFunction(type), mAffectBody(affectBody) {}