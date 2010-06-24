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

using namespace OpenEngine::Display;
using namespace OpenEngine::Scene;

class ShadowLight {
public:
    Camera* lightCam;
    PerspectiveViewingVolume* lightPersp;
    TransformationNode* lightTrans;

    ShadowLight(Camera* lc, PerspectiveViewingVolume* p, TransformationNode* t);
};


#endif // _OE_SHADOW_LIGHT_H_
