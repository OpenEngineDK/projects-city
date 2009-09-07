/*
 *  Building.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 04/09/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#ifndef _CITY_BUILDING_H_
#define _CITY_BUILDING_H_

#include <Math/Vector.h>
#include <Geometry/FaceSet.h>
#include <Math/RandomGenerator.h>

using namespace OpenEngine::Math;
using namespace OpenEngine::Geometry;

class Building {
  
    Vector<2,float> pos;
    RandomGenerator* rg;
    
public:
    Building(Vector<2,float> pos, RandomGenerator* rg);
    
    void AddToFaceSet(FaceSet* fs);
};

#endif