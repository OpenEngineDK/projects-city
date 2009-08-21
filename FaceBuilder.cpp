/*
 *  FaceBuilder.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 21/08/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#include "FaceBuilder.h"

FaceBuilder::FaceBuilder() {
    fs = new FaceSet();
    mat = new Material();
}


void FaceBuilder::AddSquare(Vector<3,float> a, Vector<3,float> b,
                            Vector<3,float> c, Vector<3,float> d) {

    Face *f1 = new Face(a,b,c);
    
    Vector<3,float> norm = (b-a) % (c-a);
    norm.Normalize();
    
    f1->norm[0] = norm;
    f1->norm[1] = norm;
    f1->norm[2] = norm;
    
    f1->colr[0] = colr;
    f1->colr[1] = colr;
    f1->colr[2] = colr;
    f1->mat = MaterialPtr(mat);
    
    fs->Add(FacePtr(f1));
    
    Face *f2 = new Face(a,c,d);
    f2->colr[0] = colr;
    f2->colr[1] = colr;
    f2->colr[2] = colr;
    
    f2->norm[0] = norm;
    f2->norm[1] = norm;
    f2->norm[2] = norm;
    
    f2->mat = MaterialPtr(mat);
    
    fs->Add(FacePtr(f2));
    
}

FaceSet* FaceBuilder::GetFaceSet() {
    return fs;
}

FaceSet* FaceBuilder::MakeABox(Vector<3,float> origin, Vector<3,float> size) {
    
    float width = size.Get(0);
    float depth = size.Get(1);
    float height = size.Get(2);
    
    float x = origin.Get(0);
    float y = origin.Get(1);
    float z = origin.Get(2);
    
    FaceBuilder fb;    
    fb.colr = Vector<4,float>(1,0,0,0);
    
    //fb.mat->specular = Vector<4,float>(.3,.3,.3,1);
    //fb.mat->shininess = 1;
    
    Vector<3,float> a = Vector<3,float>(x,y,z);
    Vector<3,float> b = Vector<3,float>(x,height,z);
    Vector<3,float> c = Vector<3,float>(width,height,z);
    Vector<3,float> d = Vector<3,float>(width,y,z);
    Vector<3,float> e = Vector<3,float>(width,height,-depth);
    Vector<3,float> f = Vector<3,float>(width,y,-depth);
    Vector<3,float> g = Vector<3,float>(x,height,-depth);
    Vector<3,float> h = Vector<3,float>(x,y,-depth);
    
    
    
    fb.AddSquare( a, b, c, d);
    fb.AddSquare( d, c, e, f);
    
    fb.AddSquare( f, e, g, h);
    fb.AddSquare( h, g, b, a);
    
    fb.AddSquare( c, b, g, e);
    fb.AddSquare( a, h, f, d);
    
    
    return fb.GetFaceSet();
}

FaceSet* FaceBuilder::MakeABox(float width, float depth, float height) {
    return MakeABox(Vector<3,float>(0,0,0),Vector<3,float>(width,depth,height));
}
