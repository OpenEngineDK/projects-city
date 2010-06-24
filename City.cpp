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
#include <Utils/MeshCreator.h>
#include <Scene/TransformationNode.h>
#include <Scene/SceneNode.h>
#include "Building.h"

using namespace OpenEngine::Math;
using namespace OpenEngine::Utils;



City::City() {

    //fs = new FaceSet();
    grid = new CityGrid(30,30);
    
    Build();
}

void City::Build() {
    node = new SceneNode();

    MeshNode* plane = new MeshNode();
    MeshNode* plane2 = new MeshNode();


    Vector<2,int> size = grid->GetSize();
    
    TransformationNode *planeT = new TransformationNode();
    planeT->AddNode(plane);
    node->AddNode(planeT);

    TransformationNode *planeT2 = new TransformationNode();
    planeT2->AddNode(plane2);
    //planeT->AddNode(planeT2);
    
    planeT->SetPosition(Vector<3,float>(15*3,0,15*3));
    planeT2->SetRotation(Quaternion<float>(3.14,Vector<3,float>(0,0,1)));

    plane->SetMesh(MeshCreator::CreatePlane(10*10, 1, Vector<3,float>(1,0,0)));
    plane2->SetMesh(MeshCreator::CreatePlane(10*10, 1, Vector<3,float>(1,0,0)));

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
