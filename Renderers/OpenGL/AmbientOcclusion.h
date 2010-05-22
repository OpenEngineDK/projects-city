// Screen Space Ambient Occlusion post processing module
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OPENGL_SCREEN_SPACE_AMBIENT_OCCLUSSION_H_
#define _OPENGL_SCREEN_SPACE_AMBIENT_OCCLUSSION_H_

#include <Renderers/IRenderer.h>
#include <Scene/ISceneNodeVisitor.h>
#include <Resources/IShaderResource.h>
#include <Meta/OpenGL.h>

namespace OpenEngine {
    namespace Scene {
        class MeshNode;
        class TransformationNode;
    }
namespace Renderers {
namespace OpenGL {

using Scene::ISceneNodeVisitor;
using Scene::MeshNode;
using Scene::TransformationNode;
using Resources::IShaderResourcePtr;

class AmbientOcclusion 
    : public IListener<RenderingEventArg>, public ISceneNodeVisitor {    
private:
    class Initializer: public IListener<RenderingEventArg> {
    private:
        AmbientOcclusion& ao;
    public:
        Initializer(AmbientOcclusion& ao): ao(ao) {}
        virtual ~Initializer() {}
        void Handle(RenderingEventArg arg) {
            ao.Initialize(arg);
        }
    };
    friend class Initializer;
    Initializer init;

    // framebuffer and texture magic
    GLuint fbo;
    GLuint normals;
    GLuint depth; 
    GLuint ao;
    IShaderResourcePtr normalShader, aoShader;
    void Initialize(RenderingEventArg arg);
public:
    AmbientOcclusion();
    virtual ~AmbientOcclusion();

    void VisitMeshNode(MeshNode* node);
    void VisitTransformationNode(TransformationNode* node);
    void Handle(RenderingEventArg arg);

    void AttachTo(IRenderer& renderer);
};

} // NS OpenGL
} // NS Renderers
} // NS OpenEngine

#endif // _OPENGL_SCREEN_SPACE_AMBIENT_OCCLUSSION_H_

