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
#include <Math/RandomGenerator.h>

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
using Math::RandomGenerator;
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
    unsigned int width, height;
    GLuint fbo, normals, depth, ao, scene, blur;
    ITexture2DPtr normtex, depthtex, aotex, whitetex, scenetex;
    IShaderResourcePtr normalShader, aoShader, blurXShader, blurYShader, mergeShader;

    bool enabled, doBlur, doNormals, merge;
    float radius, linearAtt, contrast, rays, bias, steps;

    RandomGenerator gen;

    void Initialize(RenderingEventArg arg);

    void Quad();
public:
    AmbientOcclusion();
    virtual ~AmbientOcclusion();

    void VisitMeshNode(MeshNode* node);
    void VisitTransformationNode(TransformationNode* node);
    void Handle(RenderingEventArg arg);

    void AttachTo(IRenderer& renderer);

    void SetRadius(float radius);
    float GetRadius();
    void SetLinearAttenuation(float linearAtt);
    float GetLinearAttenuation();
    void SetContrast(float contrast);
    float GetContrast();
    void SetNumOfRays(float rays);
    float GetNumOfRays();
    void SetAngleBias(float bias);
    float GetAngleBias();
    void SetNumOfSteps(float steps);
    float GetNumOfSteps();
    
    void SetEnabled(bool enabled);
    bool IsEnabled();

    void SetBlur(bool doBlur);
    bool GetBlur();

    void SetMerge(bool merge);
    bool GetMerge();
    void SetDrawNormals(bool doNormals);
    bool GetDrawNormals();
};

} // NS OpenGL
} // NS Renderers
} // NS OpenEngine

#endif // _OPENGL_SCREEN_SPACE_AMBIENT_OCCLUSSION_H_

