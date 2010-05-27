/*
 *  Building.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 04/09/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#include "Building.h"
#include <Geometry/FaceBuilder.h>
#include <Utils/MeshCreator.h>

using namespace OpenEngine::Utils;

Building::Building(Vector <2,float> pos, RandomGenerator* rg) : pos(pos), rg(rg) {
    node = new MeshNode();
    Build();
}

//void Building::AddToFaceSet(FaceSet* fs) {
void Building::Build() {
    FaceBuilder::FaceState st;
    st.color = Vector<4,float>(0,1,0,1);
    st.mat->diffuse = Vector<4,float>(.5,.5,.5,1.0);
    
    
    
    Vector<3,float> plane(pos[0],
                           0,
                           pos[1]);
    
    /*Vector<3,float> pos = Vector<3,float>(30*rg.UniformFloat(0, 10),
     0, 
     -30*rg.UniformFloat(0, 10));
     */
    
    int sections = 1;//rg->UniformInt(1, 3);
    
    float maxW=30,maxD=30;
    float minW=20,minD=20;
    float minH=20;
    
    for (int i=0;i<sections;i++) {
        Vector<3,float> size = Vector<3,float>(rg->UniformFloat(minW, maxW),
                                               rg->UniformFloat(minD, maxD),
                                               rg->UniformFloat(minH, 40));

        plane[1] += size[2]/2;
        
        //node->SetMesh(MeshCreator::CreateSimpleBox(size, Vector<3,float>(1,0,0)));
        node->SetMesh(MeshCreator::CreateCube(size[0],1, Vector<3,float>(0.5)));
        //FaceBuilder::MakeABox(fs, st, plane, size);

        maxW = size[0];
        maxD = size[1];
        minW = maxW/2;
        minD = maxD/2;
        minH = size[2]/2;
        
        
        
    }
}
    
ISceneNode* Building::GetNode() {
    return node;
}    

