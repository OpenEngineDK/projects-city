/*
 *  City.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 21/08/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#include "City.h"
#include <Geometry/FaceBuilder.h>
#include <Math/RandomGenerator.h>
#include <Scene/TransformationNode.h>
#include <Scene/SceneNode.h>
#include "Building.h"

using namespace OpenEngine::Math;



City::City() {

    //fs = new FaceSet();
    grid = new CityGrid(30,30);
    
    Build();
}

void City::Build() {
    node = new SceneNode();
    
    Vector<2,int> size = grid->GetSize();
    
    RandomGenerator rg;
    FaceBuilder::FaceState st;
    st.color[3] = 1;
    st.color[0] = 1;
    
    for (int i=0;i<size[0];i++) {
        for (int j=0;j<size[1];j++) {            
            TransformationNode *tnode = new TransformationNode();
            Vector<2,float> p = grid->PositionForBuilding(i,j);
            Building b(p,&rg);
            //FaceSet* fs = new FaceSet();
            //b.AddToFaceSet(fs);
            tnode->AddNode(b.GetNode());
            tnode->SetPosition(Vector<3,float>(p[0],0,p[1]));
            //delete fs;
            node->AddNode(tnode);
        }
    }
    //node = new GeometryNode(fs);    
}

CityGrid* City::GetGrid() {
    return grid;
}

ISceneNode* City::GetNode() {
    return node;
}
