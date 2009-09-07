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
    
    Vector<3,float> norm = (c-a) % (b-a);
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

void FaceBuilder::AddSquare(FaceSet* fs, FaceState state,
                            Vector<3,float> a, Vector<3,float> b,
                            Vector<3,float> c, Vector<3,float> d) {
    
    Face *f1 = new Face(a,b,c);
    
    Vector<3,float> norm = (c-a) % (b-a);
    norm.Normalize();
    
    f1->norm[0] = norm;
    f1->norm[1] = norm;
    f1->norm[2] = norm;
    
    f1->colr[0] = state.color;
    f1->colr[1] = state.color;
    f1->colr[2] = state.color;
    //f1->mat = MaterialPtr(mat);
    
    fs->Add(FacePtr(f1));
    
    Face *f2 = new Face(a,c,d);
    f2->colr[0] = state.color;
    f2->colr[1] = state.color;
    f2->colr[2] = state.color;
    
    f2->norm[0] = norm;
    f2->norm[1] = norm;
    f2->norm[2] = norm;
    
    //f2->mat = MaterialPtr(mat);
    
    fs->Add(FacePtr(f2));
    
}


FaceSet* FaceBuilder::GetFaceSet() {
    return fs;
}

void FaceBuilder::MakeABox(FaceSet* fs, FaceState state, Vector<3,float> origin, Vector<3,float> size) {
    
    float width = size.Get(0)/2;
    float depth = size.Get(1)/2;
    float height = size.Get(2)/2;
    
    float x = origin.Get(0);
    float y = origin.Get(1);
    float z = origin.Get(2);
    

    
    //fb.mat->specular = Vector<4,float>(.3,.3,.3,1);
    //fb.mat->shininess = 1;
    
    Vector<3,float> a = Vector<3,float>(x-width,y-height,z+depth);
    Vector<3,float> b = Vector<3,float>(x-width,y+height,z+depth);
    Vector<3,float> c = Vector<3,float>(x+width,y+height,z+depth);
    Vector<3,float> d = Vector<3,float>(x+width,y-height,z+depth);
    Vector<3,float> e = Vector<3,float>(x+width,y+height,z-depth);
    Vector<3,float> f = Vector<3,float>(x+width,y-height,z-depth);
    Vector<3,float> g = Vector<3,float>(x-width,y+height,z-depth);
    Vector<3,float> h = Vector<3,float>(x-width,y-height,z-depth);
    
    
    
    AddSquare(fs, state, a, b, c, d);
    AddSquare(fs, state, d, c, e, f);
    
    AddSquare(fs, state, f, e, g, h);
    AddSquare(fs, state, h, g, b, a);
    
    AddSquare(fs, state, c, b, g, e);
    AddSquare(fs, state, h, a, d, f);
}


