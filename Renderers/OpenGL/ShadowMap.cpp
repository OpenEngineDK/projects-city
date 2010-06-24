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

#include <Utils/TransformationNodeInspector.h>
#include <Utils/CameraInspector.h>
#include <Utils/PerspectiveViewingVolumeInspector.h>


#include <Logging/Logger.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGL {

using namespace Display;
using namespace Scene;
using namespace Geometry;
using namespace Resources;
using namespace Utils::Inspection;

#define SHADOW_MAP_RATIO 2


ShadowMap::ShadowMap(ShadowLight* l) 
  : init(Initializer(*this)),
    light(l) {
    shadowsEnabled = true;
    shadowAmount = 0.5;
    skyColor = Vector<3,float>(1);
    groundColor = Vector<3,float>(0.0);
    showThumb = false;
    showFrustum = true;
    poff = 1.1f;
    punits = 4.0f;
}

    
void ShadowMap::VisitTransformationNode(TransformationNode* node) {
    // push transformation matrix
    Matrix<4,4,float> m = node->GetTransformationMatrix();
    float f[16];
    m.ToArray(f);
    glPushMatrix();
    glMultMatrixf(f);

    glMatrixMode(GL_TEXTURE);
    glActiveTextureARB(GL_TEXTURE7);
    glPushMatrix();
    glMultMatrixf(f);

    glMatrixMode(GL_MODELVIEW);
    // traverse sub nodes
    node->VisitSubNodes(*this);

    // pop transformation matrix
    glPopMatrix();
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

}




// Loading shader function

void ShadowMap::generateShadowFBO(RenderingEventArg arg) {
	int shadowMapWidth = SHADOW_MAP_RATIO * arg.canvas.GetWidth();
	int shadowMapHeight = SHADOW_MAP_RATIO * arg.canvas.GetHeight();

    IRenderer& renderer = arg.renderer;
    Vector<2,int> dims(shadowMapWidth,shadowMapHeight);

    fb = new FrameBuffer(dims,0,true);
    renderer.BindFrameBuffer(fb);
}



void ShadowMap::SetTextureMatrix() {    
    IViewingVolume* vv = light->lightCam;
    Matrix<4,4,float> B(.5, .0, .0,  .0,
                        .0, .5, .0,  .0,
                        .0, .0, .5,  .0,
                        .5, .5, .5, 1.0);
    Matrix<4,4,float> Vl = vv->GetViewMatrix();
    Matrix<4,4,float> Pl = vv->GetProjectionMatrix();    
    Matrix<4,4,float> T = Vl*Pl*B; // The order is reversed T = B*Pl*Vl 
                                   // M is implicit (we add M in VisitTransformationNode)
    float T_arr[16];
    T.ToArray(T_arr);
	
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
    glLoadMatrixf(T_arr);
	
	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void ShadowMap::Initialize(RenderingEventArg arg) {

    generateShadowFBO(arg);
    
    // Shader
    shadowShader = ResourceManager<IShaderResource>::
        Create("projects/city/shaders/Shadow2.glsl");
    shadowShader->SetTexture("ShadowMap",fb->GetDepthTexture());
    shadowShader->SetUniform("ShadowAmount",shadowAmount);
    shadowShader->Load();

}

void ShadowMap::MakeMap2(RenderingEventArg arg) {}

void ShadowMap::MakeMap(RenderingEventArg arg) {
    int w = arg.canvas.GetWidth();
    int h = arg.canvas.GetHeight();



    glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
    CHECK_FOR_GL_ERROR();

    //First step: Render from the light POV to a FBO, story depth values only
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fb->GetID());	//Rendering offscreen

    //Using the fixed pipeline to render to the depthbuffer
    glUseProgramObjectARB(0);

    // In the case we render the shadowmap to a higher resolution, the
    // viewport must be modified accordingly.
    glViewport(0, 0,
               w * SHADOW_MAP_RATIO,
               h * SHADOW_MAP_RATIO);

    // Clear previous frame values
    glClear( GL_DEPTH_BUFFER_BIT);

    //Disable color rendering, we only want to write to the Z-Buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 

    // We offset the polygons to avoid self shadowing
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(poff, punits);

    // Apply the viewingvolume (modelview and projection)
    arg.renderer.ApplyViewingVolume(*(light->lightCam));

    // Culling switching, rendering only backface, also to avoid self-shadowing
    glCullFace(GL_FRONT);

    // Draw the scene
    arg.canvas.GetScene()->Accept(*this); 

    // Remember to reset some of the state!
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_CULL_FACE);
    CHECK_FOR_GL_ERROR();
}

void ShadowMap::Handle(RenderingEventArg arg) {
    CHECK_FOR_GL_ERROR();

    int w = arg.canvas.GetWidth();
    int h = arg.canvas.GetHeight();


    this->arg = &arg;

    if (arg.renderer.GetCurrentStage() == IRenderer::RENDERER_INITIALIZE) {
        logger.info << "initialize NOW!" << logger.end;
        Initialize(arg);
        return;
    } else if (arg.renderer.GetCurrentStage() != IRenderer::RENDERER_PROCESS)
        return;
    
    MakeMap(arg);

	//Save modelview/projection matrice into texture7, also add a bias
	SetTextureMatrix();

    // Step 2

	// Now rendering from the camera POV, using the FBO to generate shadows
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	glViewport(0,0,w,h);
	
	//Enabling color write (previously disabled for light POV z-buffer rendering)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
	
	// Clear previous frame values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
    
    // Update the position of GL_LIGHT0 to the same as the viewingvolume.
    light->Update();    

    // Set the camera
    arg.renderer.ApplyViewingVolume(*(arg.canvas.GetViewingVolume()));
    
    // We need to setup the texture for depth compare
    glBindTexture(GL_TEXTURE_2D,fb->GetDepthTexture()->GetID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);

    // Set uniforms, and set the shader program
    if (shadowsEnabled) {
        shadowShader->SetUniform("ShadowAmount", shadowAmount);
        shadowShader->ApplyShader();
    }
   
    // Render the scene
    arg.canvas.GetScene()->Accept(*this); 

    if (shadowsEnabled)
        shadowShader->ReleaseShader();
	
    glUseProgramObjectARB(0);
    glActiveTextureARB(GL_TEXTURE0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);

    glDisable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_TEXTURE_2D);

    if (showFrustum) drawLightFrustum();
    if (showThumb) drawThumb(arg);
}    
void ShadowMap::drawThumb(RenderingEventArg arg) {
    int w = arg.canvas.GetWidth();
    int h = arg.canvas.GetHeight();

    OrthogonalViewingVolume volume(1, 10,
                                   -w/2, w/2,
                                   h/2, -h/2);
    arg.renderer.ApplyViewingVolume(volume);

    glColor4f(1,1,1,1);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
       
    glActiveTextureARB(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,fb->GetDepthTexture()->GetID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);
    
    
    glEnable(GL_TEXTURE_2D);
    glTranslated(0,0,-1);    

    Quad(w/2,h/2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);


    glDisable(GL_TEXTURE_2D);
      
}

void ShadowMap::drawLightFrustum() {
    glPushMatrix();
    Matrix<4,4,float> matrix = light->lightCam->GetViewMatrix().GetInverse();
    float f[16] = {0};
    matrix.ToArray(f);
    glMultMatrixf(f);
    CHECK_FOR_GL_ERROR();

    float near = light->lightPersp->GetNear();
    float far = light->lightPersp->GetFar();

    float right,left,top,bottom;
    float fright,fleft,ftop,fbottom;
    
    float t,ty,tx;
    t = light->lightPersp->GetFOV()*0.5;
    ty = tan(t);
    tx = ty * light->lightPersp->GetAspect();
    
    right = tx*near;
    left = -right;
    top = ty*near;
    bottom = -top;


    fright = tx*far;
    fleft = -fright;
    ftop = ty*far;
    fbottom = -ftop;

    near = -near;
    far = -far;
    
	glBegin(GL_LINE_LOOP);
    glVertex3f(left,bottom,near);
    glVertex3f(left,top,near);
    glVertex3f(right,top,near);
    glVertex3f(right,bottom,near);
    glEnd();

	glBegin(GL_LINE_LOOP);
    glVertex3f(fleft,fbottom,far);
    glVertex3f(fleft,ftop,far);
    glVertex3f(fright,ftop,far);
    glVertex3f(fright,fbottom,far);
    glEnd();
    
    
    glBegin(GL_LINES);
    glVertex3f(left,bottom,near);     glVertex3f(fleft,fbottom,far);   
    glVertex3f(left,top,near);        glVertex3f(fleft,ftop,far);      
    glVertex3f(right,top,near);       glVertex3f(fright,ftop,far);     
    glVertex3f(right,bottom,near);    glVertex3f(fright,fbottom,far);  
                                     
    glEnd();

    glPopMatrix();

}


void ShadowMap::Quad(int width, int height) {
    const unsigned int z = 0.0;
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 0.0);    glVertex3i(width, 0, z);
    glTexCoord2f(1.0, 1.0);    glVertex3i(width, height, z);
    glTexCoord2f(0.0, 1.0);    glVertex3i(0, height, z);
    glTexCoord2f(0.0, 0.0);    glVertex3i(0, 0, z);
    glEnd();
}         


void ShadowMap::AttachTo(IRenderer& renderer) {
    renderer.InitializeEvent().Attach(init);
    renderer.ProcessEvent().Attach(*this);
}

ValueList ShadowMap::Inspection() {
    ValueList values;

    /* Shadows Enabled */ {
        RWValueCall<ShadowMap, bool> *v
            = new RWValueCall<ShadowMap, bool>(*this,
                                               &ShadowMap::GetShadowsEnabled,
                                               &ShadowMap::SetShadowsEnabled);
        v->name = "enabled";
        values.push_back(v);
    }
    /* Show Frustum */ {
        RWValueCall<ShadowMap, bool> *v
            = new RWValueCall<ShadowMap, bool>
            (*this,
             &ShadowMap::GetShowFrustum,
             &ShadowMap::SetShowFrustum);
        v->name = "frustum";
        values.push_back(v);
    }
    /* Show Thumb */ {
        RWValueCall<ShadowMap, bool> *v
            = new RWValueCall<ShadowMap, bool>
            (*this,
             &ShadowMap::GetShowThumb,
             &ShadowMap::SetShowThumb);
        v->name = "thumb";
        values.push_back(v);
    }
    /* Shadow Amount */ {
        RWValueCall<ShadowMap, float> *v
            = new RWValueCall<ShadowMap, float>(*this,
                                               &ShadowMap::GetShadowAmount,
                                               &ShadowMap::SetShadowAmount);
        v->name = "shadowamount";
        v->properties[MIN] = 0;
        v->properties[MAX] = 1;
        v->properties[STEP] = 0.01;
        values.push_back(v);
    }
    /* Polygon Offset */ {
        RWValueCall<ShadowMap, float> *v
            = new RWValueCall<ShadowMap, float>(*this,
                                               &ShadowMap::GetPolygonOffset,
                                               &ShadowMap::SetPolygonOffset);
        v->name = "polygonoffset";
        values.push_back(v);
    }
    /* Polygon Offset Units */ {
        RWValueCall<ShadowMap, float> *v
            = new RWValueCall<ShadowMap, float>(*this,
                                               &ShadowMap::GetPolygonUnits,
                                               &ShadowMap::SetPolygonUnits);
        v->name = "polygonunits";
        values.push_back(v);
    }    
    // /* Sky Color */ {
    //     RWValueCall<ShadowMap, Vector<3,float> > *v
    //         = new RWValueCall<ShadowMap, Vector<3,float> >
    //         (*this,
    //          &ShadowMap::GetSkyColor,
    //          &ShadowMap::SetSkyColor);
    //     v->name = "skycolor";
    //     v->isColor = true;
    //     values.push_back(v);
    // }
    // /* Ground Color */ {
    //     RWValueCall<ShadowMap, Vector<3,float> > *v
    //         = new RWValueCall<ShadowMap, Vector<3,float> >
    //         (*this,
    //          &ShadowMap::GetGroundColor,
    //          &ShadowMap::SetGroundColor);
    //     v->name = "groundcolor";
    //     v->isColor = true;
    //     values.push_back(v);
    // }

    ValueList lightCI = Inspect(light->lightCam);
    ValueList perspI = Inspect(light->lightPersp);


    values.merge(lightCI);
    values.merge(perspI);


    return values;
}

bool ShadowMap::GetShadowsEnabled() { return shadowsEnabled; }
void ShadowMap::SetShadowsEnabled(bool e){ shadowsEnabled = e; }

float ShadowMap::GetShadowAmount() { 
    return shadowAmount;
}
void ShadowMap::SetShadowAmount(float a){ 
    shadowAmount = a;
}

Vector<3,float> ShadowMap::GetSkyColor() {return skyColor; }
Vector<3,float> ShadowMap::GetGroundColor() {return groundColor; }

void ShadowMap::SetSkyColor(Vector<3,float> v) {skyColor = v;}
void ShadowMap::SetGroundColor(Vector<3,float> v) {groundColor = v;}

}
}
namespace Utils {
namespace Inspection {
    
ValueList Inspect(Renderers::OpenGL::ShadowMap *map) {
    return map->Inspection();
}
    
}
}


}
