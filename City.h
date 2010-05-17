/*
 *  City.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 21/08/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#ifndef _CITY_H_
#define _CITY_H_

#include <Scene/ISceneNode.h>
#include <Scene/GeometryNode.h>
#include <Geometry/FaceSet.h>
#include "CityGrid.h"

using namespace OpenEngine::Scene;
using namespace OpenEngine::Geometry;

class City {

    ISceneNode* node;
    
    CityGrid* grid;
    
    //FaceSet* fs;
    
    void Build();
    
public:
    City();
    
    CityGrid* GetGrid();
    
    ISceneNode* GetNode();
    
};

#endif //_CITY_H_
