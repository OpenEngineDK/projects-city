// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_SHADOW_MAP_H_
#define _OE_SHADOW_MAP_H_

#include <Scene/ISceneNodeVisitor.h>
#include <Meta/OpenGL.h>
#include <Renderers/IRenderer.h>
#include <Math/Matrix.h>
#include <Display/Camera.h>
#include <Display/PerspectiveViewingVolume.h>
#include <Resources/IShaderResource.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Utils/IInspector.h>
#include "../../ShadowLight.h"

namespace OpenEngine {
namespace Renderers {
namespace OpenGL {

using Scene::ISceneNodeVisitor;
using namespace Display;

// class ShadowMap : public IListener<RenderingEventArg>,
//                    public ISceneNodeVisitor {
class ShadowMap : public RenderingView{

    class Initializer : public IListener<RenderingEventArg> {
        ShadowMap& sm;
    public:
        Initializer(ShadowMap& sm) : sm(sm) {}
        void Handle(RenderingEventArg arg) {sm.Initialize(arg);}
    };
    friend class Initializer;
    Initializer init;
    
    void Initialize(RenderingEventArg arg);

    FrameBuffer* fb;
    Camera* viewVol;
    
    ShadowLight* light;

    IShaderResourcePtr shadowShader;
    void Quad(int,int);
    
    void MakeMap2(RenderingEventArg arg);
    void MakeMap(RenderingEventArg arg);

    void drawObjects(RenderingEventArg);
    void drawLightFrustum();
    void drawThumb(RenderingEventArg);

    void generateShadowFBO(RenderingEventArg);

    void setTextureMatrix();

    bool shadowsEnabled;
    float shadowAmount;
    float poff;
    float punits;

    bool showThumb,showFrustum;

    Vector<3,float> skyColor;
    Vector<3,float> groundColor;
public:
    ShadowMap(ShadowLight*);
    void Handle(RenderingEventArg arg);

    void AttachTo(IRenderer& renderer);    

    // void VisitMeshNode(MeshNode*); 
    void VisitTransformationNode(TransformationNode*);
    Utils::Inspection::ValueList Inspection();

    bool GetShadowsEnabled();
    void SetShadowsEnabled(bool e);

    float GetShadowAmount();
    void SetShadowAmount(float a);

    float GetPolygonOffset() {return poff; }
    void SetPolygonOffset(float a) {poff = a;}

    float GetPolygonUnits() {return punits; }
    void SetPolygonUnits(float a) {punits = a;}

    Vector<3,float> GetSkyColor();
    Vector<3,float> GetGroundColor();

    void SetSkyColor(Vector<3,float>);
    void SetGroundColor(Vector<3,float>);

    void SetShowThumb(bool s) {showThumb = s;}
    bool GetShowThumb() {return showThumb;}

    void SetShowFrustum(bool s) {showFrustum = s;}
    bool GetShowFrustum() {return showFrustum;}

};

} // NS OpenGL
} // NS Renderers
namespace Utils {
namespace Inspection {
    
    ValueList Inspect(Renderers::OpenGL::ShadowMap *map);
    
}
}
} // NS OpenEngine


#endif // _OE_SHADOW_MAP_H_
