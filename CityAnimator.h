/*
 *  CityAnimator.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 04/09/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#ifndef _CITY_ANIMATOR_H_
#define _CITY_ANIMATOR_H_

#include "City.h"

#include <Display/Camera.h>
#include <Core/EngineEvents.h>
#include <Utils/Timer.h>
#include <Math/RandomGenerator.h>
#include <Scene/TransformationNode.h>
#include <Core/IListener.h>

using namespace OpenEngine::Display;
using namespace OpenEngine::Core;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Math;
using namespace OpenEngine::Scene;

class CityAnimator : public IListener<ProcessEventArg> {
    
    bool first;
    
    Vector<3,float> lookPos;
    Vector<3,float> camPos;
    
    Timer t;
    RandomGenerator rg;
    City* city;
    Camera* camera;
    TransformationNode* trans;
    
public:
    CityAnimator(City* c, Camera *cam, TransformationNode* tn);
    void Handle(ProcessEventArg arg);
    
};

#endif
