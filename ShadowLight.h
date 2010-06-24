// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_SHADOW_LIGHT_H_
#define _OE_SHADOW_LIGHT_H_

#include <Display/Camera.h>
#include <Display/PerspectiveViewingVolume.h>
#include <Scene/TransformationNode.h>
#include <Geometry/Light.h>

using namespace OpenEngine::Display;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Geometry;

class ShadowLight {
public:
    Camera* lightCam;
    PerspectiveViewingVolume* lightPersp;
    TransformationNode* lightTrans;
    Light *light;

    ShadowLight(Camera* lc, PerspectiveViewingVolume* p, TransformationNode* t, Light* l);
    void Update();
};


#endif // _OE_SHADOW_LIGHT_H_
