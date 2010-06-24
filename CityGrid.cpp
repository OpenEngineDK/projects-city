/*
 *  CityGrid.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 04/09/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#include "CityGrid.h"

CityGrid::CityGrid(int w, int d) : size(w,d) {
    bsize = 3;
    margin = 2;
}

Vector<2,float> CityGrid::PositionForBuilding(int x, int y) {

    
    
    return Vector<2,float>(x*bsize+margin*int(x/2),
                           y*bsize+margin*int(y/4));
    
}

Vector<2,float> CityGrid::PositionForCrossing(int x, int y) {
    
    
    
    return Vector<2,float>(x*bsize*2+(x-1)*margin-margin/2,
                           y*bsize*4+(y-1)*margin-margin/2);
    
}

Vector<2,int> CityGrid::GetSize() {
    return size;
}


Vector<2,int> CityGrid::GetRoadSize() {
    Vector<2,int> ret = size;
    
    ret[0] /= 2;
    ret[1] /= 4;
    
    return ret;
}
