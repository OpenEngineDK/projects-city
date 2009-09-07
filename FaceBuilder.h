/*
 *  FaceBuilder.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 21/08/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#include <Geometry/FaceSet.h>
#include <Geometry/Box.h>

using namespace OpenEngine::Geometry;


class FaceBuilder {
    
    
public:
    
    struct FaceState {
        Vector<4,float> color;    
    };
    
    static void MakeABox(FaceSet* fs, FaceState state, Vector<3,float> origin, Vector<3,float> size);    
    static void AddSquare(FaceSet* fs, FaceState state, Vector<3,float>,Vector<3,float>,Vector<3,float>,Vector<3,float>);

private:
    FaceSet *fs;
    Vector<4,float> colr;
    Material* mat;
    
    FaceBuilder();
    void AddSquare(Vector<3,float>,Vector<3,float>,Vector<3,float>,Vector<3,float>);
    FaceSet* GetFaceSet();
};