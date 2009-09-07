/*
 *  CityGrid.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 04/09/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#ifndef _CITY_GRID_H_
#define _CITY_GRID_H_

#include <Math/Vector.h>

using namespace OpenEngine::Math;

class CityGrid { 
  
    Vector<2,int> size;
    
public:
    CityGrid(int w, int d);
    
    Vector<2,int> GetSize();
    Vector<2,int> GetRoadSize();
    
    Vector<2,float> PositionForBuilding(int x, int y);
    Vector<2,float> PositionForCrossing(int x, int y);
    
};

#endif