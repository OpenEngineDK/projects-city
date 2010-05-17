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
#include <Scene/MeshNode.h>

using namespace OpenEngine::Math;
using namespace OpenEngine::Scene;

class Building {
  
    Vector<2,float> pos;
    RandomGenerator* rg;
    MeshNode* node;
    
public:
    Building(Vector<2,float> pos, RandomGenerator* rg);
    
    //void AddToFaceSet(FaceSet* fs);
    void Build();
    ISceneNode* GetNode();
};

#endif
