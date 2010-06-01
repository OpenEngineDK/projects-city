#include "ShadowMap.h"

#include <Scene/MeshNode.h>
#include <Scene/TransformationNode.h>

#include <Geometry/Mesh.h>
#include <Geometry/GeometrySet.h>

#include <Resources/DataBlock.h>
#include <Resources/FrameBuffer.h>
#include <Resources/ResourceManager.h>
#include <Resources/Texture2D.h>

#include <Display/OrthogonalViewingVolume.h>
#include <Display/PerspectiveViewingVolume.h>
#include <Display/Camera.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGL {

using namespace Display;
using namespace Scene;
using namespace Geometry;
using namespace Resources;


ShadowMap::ShadowMap(TransformationNode* tn) : init(Initializer(*this)),
                                               node (tn) {
        
}

void ShadowMap::VisitMeshNode(MeshNode* node) {
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
    
void ShadowMap::VisitTransformationNode(TransformationNode* node) {
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


void ShadowMap::Initialize(RenderingEventArg arg) {
    IRenderer& renderer = arg.renderer;
    int w = arg.canvas.GetWidth();
    int h = arg.canvas.GetHeight();
    Vector<2,int> dims(w,h);
    fb = new FrameBuffer(dims,1,true);
    renderer.BindFrameBuffer(fb);
    

    OrthogonalViewingVolume *v = new OrthogonalViewingVolume(1,1000,
                                                             w,0,
                                                             h,0);
    PerspectiveViewingVolume *p = new PerspectiveViewingVolume(-1,1);

    Camera* c = new Camera(*p);
    c->SetPosition(Vector<3,float>(0,200,200));    
    c->LookAt(100,100,0);
    viewVol = c;
    //viewVol = new PerspectiveViewingVolume();

    testShader = ResourceManager<IShaderResource>::
        Create("projects/city/shaders/Shadow1.glsl");
    testShader->SetTexture("ttex",fb->GetDepthTexture());
    //testShader->SetTexture("ttex",fb->GetTexAttachment(0));
    testShader->Load();

    debugShader = ResourceManager<IShaderResource>::
        Create("projects/city/shaders/Shadow1dbg.glsl");
    debugShader->SetTexture("dtex",fb->GetDepthTexture());
    debugShader->SetTexture("ttex",fb->GetTexAttachment(0));
    debugShader->Load();

}


void ShadowMap::Handle(RenderingEventArg arg) {
    IRenderer& renderer = arg.renderer;
    int w = arg.canvas.GetWidth();
    int h = arg.canvas.GetHeight();
    Vector<2,int> dims(w,h);

    Vector<3,float> p;    
    Quaternion<float> q;


    node->GetAccumulatedTransformations(&p,&q);
    viewVol->SetPosition(p);
    //    Matrix<4,4,float> m = ;
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb->GetID());

    renderer.ApplyViewingVolume(*viewVol);
    glClearColor(0.5,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    // shader 1 here
    arg.canvas.GetScene()->Accept(*this);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    Matrix<4,4,float> M(1);

    Matrix<4,4,float> bias(0.5, 0.0, 0.0, 0.5,
                           0.0, 0.5, 0.0, 0.5,
                           0.0, 0.0, 0.5, 0.5,
                           0.0, 0.0, 0.0, 1.0);
    //bias.Transpose();
    Matrix<4,4,float> V = viewVol->GetViewMatrix();
    Matrix<4,4,float> P = viewVol->GetProjectionMatrix();

    Matrix<4,4,float> mat = bias*M*V*P;
    float arr[15];
    mat.ToArray(arr);
    
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glMatrixMode(GL_TEXTURE);
    
    glLoadMatrixf(arr);

    glMatrixMode(GL_MODELVIEW);

    glActiveTextureARB(GL_TEXTURE0_ARB);


    //OrthogonalViewingVolume volume(-1,1,0,w,0,h);
    glClearColor(0.3,0.3,0.3,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    arg.renderer.ApplyViewingVolume(*(arg.canvas.GetViewingVolume()));    
    testShader->ApplyShader();    
    arg.canvas.GetScene()->Accept(*this);
    //Quad(w,h);
    testShader->ReleaseShader();

    OrthogonalViewingVolume volume(-1,1,0,w,0,h);
    arg.renderer.ApplyViewingVolume(volume);
    glBindTexture(GL_TEXTURE_2D, fb->GetDepthTexture()->GetID());
    debugShader->ApplyShader();
    Quad(w/2,h/2);
    debugShader->ReleaseShader();
    glBindTexture(GL_TEXTURE_2D, 0);

}

    void ShadowMap::Quad(int width, int height) {
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


void ShadowMap::AttachTo(IRenderer& renderer) {
    renderer.InitializeEvent().Attach(init);
    renderer.PostProcessEvent().Attach(*this);
}

}
}
}
