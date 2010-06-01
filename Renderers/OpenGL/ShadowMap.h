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
#include <Resources/IShaderResource.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGL {

using Scene::ISceneNodeVisitor;
using namespace Display;

class ShadowMap : public IListener<RenderingEventArg>,
                  public ISceneNodeVisitor {

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
    TransformationNode* node;

    IShaderResourcePtr testShader;
    IShaderResourcePtr debugShader;

    void Quad(int,int);

public:
    ShadowMap(TransformationNode *tn);
    void Handle(RenderingEventArg arg);

    void AttachTo(IRenderer& renderer);    

    void VisitMeshNode(MeshNode*);
    void VisitTransformationNode(TransformationNode*);
   
};

} // NS OpenGL
}
} // NS OpenEngine


#endif // _OE_SHADOW_MAP_H_
