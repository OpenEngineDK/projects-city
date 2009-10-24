/*
 *  CityAnimator.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 04/09/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#include "CityAnimator.h"
#include "CityGrid.h"

#include <Logging/Logger.h>

CityAnimator::CityAnimator(City* c, Camera* cam, TransformationNode* tn) 
: first(true),city(c), camera(cam),trans(tn) {
    t.Start();
}

void CityAnimator::Handle(ProcessEventArg arg) {
    
    //logger.info << t.GetElapsedTime() << logger.end;
    
    if (t.GetElapsedIntervals(5000000) || first) {
        t.Reset();
        first = false;
        
        CityGrid* cg = city->GetGrid();
        Vector<2,int> size = cg->GetRoadSize();
        
        int x = rg.UniformInt(0, size[0]);
        int y = rg.UniformInt(0, size[1]);
        
        Vector<2,float> cross = cg->PositionForCrossing(x,y);

        Vector<2,float> cross2 = cg->PositionForCrossing(rg.UniformInt(0,size[0]),y);
        
        
        lookPos = Vector<3,float>(cross[0],0,cross[1]);
        camPos = Vector<3,float>(cross2[0],50,cross2[1]);
        
        trans->SetPosition(lookPos);
        camera->SetPosition(camPos);
        camera->LookAt(lookPos);
        
        logger.info << "one sec" << logger.end;
    } else {
        Vector<3,float> dir =  lookPos - camPos;
        
        //logger.info << t.GetElapsedIntervals(100000)/50.0 << logger.end;
        
        Vector<3,float> currentPos = camPos + dir*t.GetElapsedIntervals(10000)/500.0;
        camera->SetPosition(currentPos);
    }
    
}