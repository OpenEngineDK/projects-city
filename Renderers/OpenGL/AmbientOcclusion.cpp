// Screen Space Ambient Occlusion post processing module
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

// #include <Renderers/OpenGL/AmbientOcclusion.h>
#include "AmbientOcclusion.h"
#include <Math/Math.h>
#include <Math/Matrix.h>
#include <Scene/ISceneNode.h>
#include <Scene/MeshNode.h>
#include <Scene/TransformationNode.h>
#include <Geometry/Mesh.h>
#include <Geometry/GeometrySet.h>
#include <Resources/DataBlock.h>
#include <Resources/ResourceManager.h>
#include <Resources/Texture2D.h>
#include <Display/OrthogonalViewingVolume.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGL {

    using namespace Scene;
    using namespace Geometry;
    using namespace Resources;
    using namespace Display;

    AmbientOcclusion::AmbientOcclusion()
        : init(Initializer(*this)),
          enabled(true),
          doBlur(false),
          radius(10.0),
          linearAtt(0.8),
          contrast(0.5),
          rays(32.0),
          bias(Math::PI / 6.0),
          steps(5.0)
    {

    }
    
    AmbientOcclusion::~AmbientOcclusion() {

    }



    void AmbientOcclusion::SetRadius(float radius) {
        this->radius = radius;
    }

    float AmbientOcclusion::GetRadius() {
        return radius;
    }

    void AmbientOcclusion::SetLinearAttenuation(float linearAtt) {
        this->linearAtt = linearAtt;
    }

    float AmbientOcclusion::GetLinearAttenuation() {
        return linearAtt;
    }

    void AmbientOcclusion::SetContrast(float contrast) {
        this->contrast = contrast;
    }

    float AmbientOcclusion::GetContrast() {
        return contrast;
    }

    void AmbientOcclusion::SetNumOfRays(float rays) {
        this->rays = rays;
    }

    float AmbientOcclusion::GetNumOfRays() {
        return rays;
    }
    void AmbientOcclusion::SetNumOfSteps(float steps) {
        this->steps = steps;
    }

    float AmbientOcclusion::GetNumOfSteps() {
        return steps;
    }
    void AmbientOcclusion::SetAngleBias(float bias) {
        this->bias = bias;
    }

    float AmbientOcclusion::GetAngleBias() {
        return bias;
    }

    void AmbientOcclusion::SetEnabled(bool enabled) {
        this->enabled = enabled;
    }

    bool AmbientOcclusion::IsEnabled() {
        return enabled;
    }

    void AmbientOcclusion::SetBlur(bool doBlur) {
        this->doBlur = doBlur;
    }

    bool AmbientOcclusion::GetBlur() {
        return doBlur;
    }

    void AmbientOcclusion::VisitMeshNode(MeshNode* node) {
        Mesh* mesh = node->GetMesh().get();
        GeometrySet* gs = mesh->GetGeometrySet().get();
        IDataBlock* v = gs->GetVertices().get();
        if (v) {
            glEnableClientState(GL_VERTEX_ARRAY);
            CHECK_FOR_GL_ERROR();
            if (v->GetID() != 0) {
                glBindBuffer(GL_ARRAY_BUFFER, v->GetID());
                glVertexPointer(v->GetDimension(), GL_FLOAT, 0, 0);
                CHECK_FOR_GL_ERROR();
            } else {
                glVertexPointer(v->GetDimension(), GL_FLOAT, 0, v->GetVoidDataPtr());
                CHECK_FOR_GL_ERROR();
            }
        }

        IDataBlock* n = gs->GetNormals().get();
        if (n) {
            glEnableClientState(GL_NORMAL_ARRAY);
            CHECK_FOR_GL_ERROR();
            if (n->GetID() != 0) {
                glBindBuffer(GL_ARRAY_BUFFER, n->GetID());
                glNormalPointer(GL_FLOAT, 0, 0);
                CHECK_FOR_GL_ERROR();
            } else {
                glNormalPointer(GL_FLOAT, 0, n->GetVoidDataPtr());
                CHECK_FOR_GL_ERROR();
            }
        }


        Indices* indices = mesh->GetIndices().get();
        GLsizei count = mesh->GetDrawingRange();
        unsigned int offset = mesh->GetIndexOffset();
        Geometry::Type type = mesh->GetType();
        
        if (indices->GetID() != 0){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->GetID());
            CHECK_FOR_GL_ERROR();
            glDrawElements(type, count, GL_UNSIGNED_INT, (GLvoid*)(offset * sizeof(GLuint)));
            CHECK_FOR_GL_ERROR();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            CHECK_FOR_GL_ERROR();
        } else {
            glDrawElements(type, count, GL_UNSIGNED_INT, indices->GetData() + offset);
            CHECK_FOR_GL_ERROR();
        }
        CHECK_FOR_GL_ERROR();
        node->VisitSubNodes(*this);
    }
    
    void AmbientOcclusion::VisitTransformationNode(TransformationNode* node) {
        // push transformation matrix
        Matrix<4,4,float> m = node->GetTransformationMatrix();
        float f[16];
        m.ToArray(f);
        glPushMatrix();
        CHECK_FOR_GL_ERROR();
        glMultMatrixf(f);
        CHECK_FOR_GL_ERROR();
        // traverse sub nodes
        node->VisitSubNodes(*this);
        CHECK_FOR_GL_ERROR();
        // pop transformation matrix
        glPopMatrix();
        CHECK_FOR_GL_ERROR();
    }
    
    void AmbientOcclusion::Initialize(RenderingEventArg arg) {
        // check for fbo support
        const std::string fboExt = "GL_EXT_framebuffer_object";
        if (glewGetExtension(fboExt.c_str()) != GL_TRUE )
            throw Exception(fboExt + " not supported");

        width = arg.canvas.GetWidth();
        height = arg.canvas.GetHeight();

        // create fbo
        glGenFramebuffersEXT(1, &fbo);

        // create normal texture (each rgb color represents a float3 vector)
        normtex = FloatTexture2DPtr(new Texture2D<float>(width, height, 4));
        normtex->SetFiltering(NONE);
        normtex->SetColorFormat(RGBA32F);
        normtex->SetMipmapping(false);
        normtex->SetCompression(false);
        normtex->SetWrapping(CLAMP_TO_EDGE);
        arg.renderer.LoadTexture(normtex);
        normals = normtex->GetID();

        // create depth texture 
        depthtex = FloatTexture2DPtr(new Texture2D<float>(width, height, 1));
        depthtex->SetFiltering(NONE);
        depthtex->SetColorFormat(DEPTH);
        depthtex->SetMipmapping(false);
        depthtex->SetCompression(false);
        depthtex->SetWrapping(CLAMP_TO_EDGE);
        arg.renderer.LoadTexture(depthtex);
        depth = depthtex->GetID();

        // create ao texture 
        aotex = FloatTexture2DPtr(new Texture2D<float>(width, height, 1));
        aotex->SetFiltering(BILINEAR);
        aotex->SetColorFormat(LUMINANCE32F);
        aotex->SetMipmapping(false);
        aotex->SetCompression(false);
        aotex->SetWrapping(CLAMP_TO_EDGE);
        arg.renderer.LoadTexture(aotex);
        ao = aotex->GetID();

        ITexture2DPtr blurtex = FloatTexture2DPtr(new Texture2D<float>(width, height, 1));
        blurtex->SetFiltering(BILINEAR);
        blurtex->SetColorFormat(LUMINANCE32F);
        blurtex->SetMipmapping(false);
        blurtex->SetCompression(false);
        blurtex->SetWrapping(CLAMP_TO_EDGE);
        arg.renderer.LoadTexture(blurtex);
        blur = blurtex->GetID();

        scenetex = UCharTexture2DPtr(new Texture2D<unsigned char>(width, height, 3));
        scenetex->SetColorFormat(RGB);
        scenetex->SetMipmapping(false);
        scenetex->SetCompression(false);
        scenetex->SetWrapping(CLAMP_TO_EDGE);
        arg.renderer.LoadTexture(scenetex);
        scene = scenetex->GetID();

        unsigned char* d = new unsigned char[3];
        d[0] = 0xFF;
        d[1] = 0xFF;
        d[2] = 0xFF;
        whitetex = UCharTexture2DPtr(new Texture2D<unsigned char>(1, 1, 3, d));        
        whitetex->SetFiltering(NONE);
        whitetex->SetColorFormat(RGB);
        whitetex->SetMipmapping(false);
        whitetex->SetCompression(false);
        whitetex->SetWrapping(REPEAT);
        arg.renderer.LoadTexture(scenetex);

        // Bind the fbo and attach normal and depth textures
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, normals, 0);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, ao, 0);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_2D, blur, 0);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depth, 0);
        CHECK_FRAMEBUFFER_STATUS();
        //bind the main back buffer again
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        
        // create and load the shader
        normalShader = ResourceManager<IShaderResource>
            ::Create("projects/city/shaders/SurfaceNormals.glsl");
        normalShader->Load();

        aoShader = ResourceManager<IShaderResource>
            ::Create("projects/city/shaders/AmbientOcclusion.glsl");
        aoShader->Load();
        aoShader->SetTexture("normals", normtex);
        aoShader->SetTexture("d", depthtex);
        
        blurXShader = ResourceManager<IShaderResource>
            ::Create("projects/city/shaders/blurX.glsl");
        blurXShader->Load();
        blurXShader->SetTexture("ao",  aotex);

        blurYShader = ResourceManager<IShaderResource>
            ::Create("projects/city/shaders/blurY.glsl");
        blurYShader->Load();
        blurYShader->SetTexture("ao",  blurtex);
        // blurYShader->SetTexture("scene",  scenetex);

        mergeShader = ResourceManager<IShaderResource>
            ::Create("projects/city/shaders/merge.glsl");
        mergeShader->Load();
        mergeShader->SetTexture("ao",  aotex);
    }

    void AmbientOcclusion::Quad() {
        const unsigned int z = 0.0;
        glBegin(GL_QUADS);
          glTexCoord2f(0.0, 0.0);
          glVertex3i(0, height, z);
          glTexCoord2f(0.0, 1.0);
          glVertex3i(0, 0, z);
          glTexCoord2f(1.0, 1.0);
          glVertex3i(width, 0, z);
          glTexCoord2f(1.0, 0.0);
          glVertex3i(width, height, z);
        glEnd();
    }         

    void AmbientOcclusion::Handle(RenderingEventArg arg) { 
        if (!enabled) return;
        glBindTexture(GL_TEXTURE_2D, scene);
        CHECK_FOR_GL_ERROR();
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, width, height, 0);
        CHECK_FOR_GL_ERROR();
        glBindTexture(GL_TEXTURE_2D, 0);

        // bind normal vector frame buffer texture
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glClearColor(0.0,0.0,0.0,1.0);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // collect normals 
        normalShader->ApplyShader();
        CHECK_FOR_GL_ERROR();
        arg.canvas.GetScene()->Accept(*this);
        CHECK_FOR_GL_ERROR();
        normalShader->ReleaseShader();

        // Vector<4,int> d(0, 0, width, height);
        // glViewport((GLsizei)d[0], (GLsizei)d[1], (GLsizei)d[2], (GLsizei)d[3]);
        OrthogonalViewingVolume volume(-1, 1, 0, width, 0, height);
        // Select The Projection Matrix
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        CHECK_FOR_GL_ERROR();
        // Reset The Projection Matrix
        glLoadIdentity();
        CHECK_FOR_GL_ERROR();
        // Setup OpenGL with the volumes projection matrix
        Matrix<4,4,float> projMatrix = volume.GetProjectionMatrix();
        float arr[16];
        projMatrix.ToArray(arr);
        glMultMatrixf(arr);
        CHECK_FOR_GL_ERROR();
        // Select the modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        CHECK_FOR_GL_ERROR();
        // Reset the modelview matrix
        glLoadIdentity();
        CHECK_FOR_GL_ERROR();
        // Get the view matrix and apply it
        Matrix<4,4,float> matrix = volume.GetViewMatrix();
        float f[16] = {0};
        matrix.ToArray(f);
        glMultMatrixf(f);
        CHECK_FOR_GL_ERROR();
        
        // setup gl quad state
        GLboolean depth    = glIsEnabled(GL_DEPTH_TEST);
        GLboolean lighting = glIsEnabled(GL_LIGHTING);
        GLboolean blending = glIsEnabled(GL_BLEND);
        GLboolean texture  = glIsEnabled(GL_TEXTURE_2D);
        GLint texenv;
        glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &texenv);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_TEXTURE_2D);

        // calculate ao
        glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
        // if (doBlur) glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
        // else glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        Matrix<4,4,float> proj = arg.canvas.GetViewingVolume()->GetProjectionMatrix();
        Matrix<4,4,float> unproj = proj.GetInverse();
        aoShader->SetUniform("proj", proj);
        aoShader->SetUniform("unproj", unproj);
        aoShader->SetTexture("normals", normtex);
        aoShader->SetTexture("d", depthtex);

        aoShader->SetUniform("sphereRad", radius);
        aoShader->SetUniform("linearAtt", linearAtt);
        aoShader->SetUniform("contrast", contrast);
        aoShader->SetUniform("rays", rays);
        aoShader->SetUniform("bias", bias);
        aoShader->SetUniform("steps", steps);

        glClearColor(1.0,1.0,1.0,1.0);
        glClear( GL_COLOR_BUFFER_BIT);
        aoShader->ApplyShader();
        CHECK_FOR_GL_ERROR();
        Quad();
        aoShader->ReleaseShader();
        CHECK_FOR_GL_ERROR();
       
        if (doBlur) {
            // blur in the x-direction
            glDrawBuffer(GL_COLOR_ATTACHMENT2_EXT);
            blurXShader->ApplyShader();
            CHECK_FOR_GL_ERROR();
            Quad();
            blurXShader->ReleaseShader();
            CHECK_FOR_GL_ERROR();
            
            // blur in the y-direction
            //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
            blurYShader->ApplyShader();
            CHECK_FOR_GL_ERROR();
            Quad();
            blurYShader->ReleaseShader();
            CHECK_FOR_GL_ERROR();     
        }

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        //mergeShader->SetTexture("scene",  whitetex);
        mergeShader->SetTexture("scene",  scenetex);
        mergeShader->ApplyShader();
        CHECK_FOR_GL_ERROR();
        Quad();
        mergeShader->ReleaseShader();
        CHECK_FOR_GL_ERROR();     

        // reset gl state
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        CHECK_FOR_GL_ERROR();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        CHECK_FOR_GL_ERROR();
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, texenv);
        if (depth)    glEnable(GL_DEPTH_TEST);
        if (lighting) glEnable(GL_LIGHTING);
        if (blending) glEnable(GL_BLEND);
        if (texture)  glEnable(GL_TEXTURE_2D);
    }

    void AmbientOcclusion::AttachTo(IRenderer& renderer) {
        renderer.InitializeEvent().Attach(init);
        renderer.PostProcessEvent().Attach(*this);
    }


} // NS OpenGL
} // NS Renderers
} // NS OpenEngine
