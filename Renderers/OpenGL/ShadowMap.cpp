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

    //#define RENDER_WIDTH 800.0
    //#define RENDER_HEIGHT 600.0
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
    poff = 1.0f;
    punits = 10.0f;
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

    int w = arg.canvas.GetWidth();
    int h = arg.canvas.GetHeight();

    
	int shadowMapWidth = w * SHADOW_MAP_RATIO;
	int shadowMapHeight = h * SHADOW_MAP_RATIO;
	

    IRenderer& renderer = arg.renderer;
    // int w = arg.canvas.GetWidth();
    // int h = arg.canvas.GetHeight();
    Vector<2,int> dims(shadowMapWidth,shadowMapHeight);
    fb = new FrameBuffer(dims,1,true);
    renderer.BindFrameBuffer(fb);
}


void ShadowMap::drawObjects(RenderingEventArg arg) {

    arg.canvas.GetScene()->Accept(*this); return;
	
	
}

void ShadowMap::setTextureMatrix() {
	// static double modelView[16];
	// static double projection[16];
	
	// // This is matrix transform every coordinate x,y,z
	// // x = x* 0.5 + 0.5 
	// // y = y* 0.5 + 0.5 
	// // z = z* 0.5 + 0.5 
	// // Moving from unit cube [-1,1] to [0,1]  
	// const GLdouble bias[16] = {	
	// 	0.5, 0.0, 0.0, 0.0, 
	// 	0.0, 0.5, 0.0, 0.0,
	// 	0.0, 0.0, 0.5, 0.0,
    //     0.5, 0.5, 0.5, 1.0};
	

    
    IViewingVolume* vv = light->lightCam;

    Matrix<4,4,float> B(.5, .0, .0,  .0,
                        .0, .5, .0,  .0,
                        .0, .0, .5,  .0,
                        .5, .5, .5, 1.0);
    Matrix<4,4,float> Vl = vv->GetViewMatrix();
    Matrix<4,4,float> Pl = vv->GetProjectionMatrix();
    
    Matrix<4,4,float> M = Vl*Pl*B; // WTF!
    
    //logger.info << M << logger.end;

    float M_arr[16];
    // float B_arr[16];
    // float Pl_arr[16];
    // float Vl_arr[16];

    M.ToArray(M_arr);
    // B.ToArray(B_arr);
    // Pl.ToArray(Pl_arr);
    // Vl.ToArray(Vl_arr);    


	// Grab modelview and transformation matrices
	// glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	// glGetDoublev(GL_PROJECTION_MATRIX, projection);
	
	
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	
	glLoadIdentity();
    
	//glLoadMatrixf(B_arr);
	
    // concatating all matrice into one.
    //glMultMatrixd (projection);
    //glMultMatrixf (Pl_arr);
    //glMultMatrixd (modelView);
    //glMultMatrixf (Vl_arr);

    glLoadMatrixf(M_arr);
	
	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void ShadowMap::Initialize(RenderingEventArg arg) {

    generateShadowFBO(arg);
    

    // FBO

    //fboId = fb->GetID();


    // Shader
    shadowShader = ResourceManager<IShaderResource>::
        Create("projects/city/shaders/Shadow2.glsl");
    shadowShader->SetTexture("ShadowMap",fb->GetDepthTexture());
    shadowShader->SetUniform("ShadowAmount",shadowAmount);
    //shadowShader->SetUniform("SkyColor",skyColor);
    //shadowShader->SetUniform("GroundColor",groundColor);
    //shadowShader->SetUniform("LightPosition",light->lightCam->GetPosition());
    shadowShader->Load();

}

void ShadowMap::MakeMap2(RenderingEventArg arg) {}

void ShadowMap::MakeMap(RenderingEventArg arg) {}

void ShadowMap::Handle(RenderingEventArg arg) {

    CHECK_FOR_GL_ERROR();

    int w = arg.canvas.GetWidth();
    int h = arg.canvas.GetHeight();


    this->arg = &arg;

    if (arg.renderer.GetCurrentStage() == IRenderer::RENDERER_INITIALIZE) {
        logger.info << "initialize NOW!" << logger.end;
        Initialize(arg);
        return;
    } 

    // We should compare the depth component only.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
    

	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0f);
	
	glEnable(GL_CULL_FACE);
    CHECK_FOR_GL_ERROR();

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);


	//First step: Render from the light POV to a FBO, story depth values only
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fb->GetID());	//Rendering offscreen
	
	//Using the fixed pipeline to render to the depthbuffer
	glUseProgramObjectARB(0);
	
	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	glViewport(0,0,w * SHADOW_MAP_RATIO,h* SHADOW_MAP_RATIO);
	
	// Clear previous frame values
	glClear( GL_DEPTH_BUFFER_BIT);
	
	//Disable color rendering, we only want to write to the Z-Buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 


    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(poff, punits);
	
    
    arg.renderer.ApplyViewingVolume(*(light->lightCam));
	//setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);
	
	// Culling switching, rendering only backface, this is done to avoid self-shadowing
	glCullFace(GL_FRONT);
	drawObjects(arg);
	
	//Save modelview/projection matrice into texture7, also add a biais

	setTextureMatrix();

    // Step 2
	

	// Now rendering from the camera POV, using the FBO to generate shadows
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	glViewport(0,0,w,h);
	
	//Enabling color write (previously disabled for light POV z-buffer rendering)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
	
	// Clear previous frame values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_FOR_GL_ERROR();

    light->lightTrans->SetPosition(light->lightCam->GetPosition());
		
    arg.renderer.ApplyViewingVolume(*(arg.canvas.GetViewingVolume()));

    glDisable(GL_POLYGON_OFFSET_FILL);

    
       
	glCullFace(GL_BACK);

    if (shadowsEnabled) {
        // shadowShader->SetUniform("ShadowAmount", shadowAmount);
        // shadowShader->SetUniform("SkyColor",skyColor);
        // shadowShader->SetUniform("GroundColor",groundColor);

        shadowShader->ApplyShader();
    }
   
    //this->ignoreMaterial = true;
	drawObjects(arg);
    //this->ignoreMaterial = false;	

    if (shadowsEnabled)
        shadowShader->ReleaseShader();
	
    glUseProgramObjectARB(0);
    glActiveTextureARB(GL_TEXTURE0);

    if (showFrustum) drawLightFrustum();
    if (showThumb) drawThumb(arg);
}    
void ShadowMap::drawThumb(RenderingEventArg arg) {
    int w = arg.canvas.GetWidth();
    int h = arg.canvas.GetHeight();

    // OrthogonalViewingVolume volume(1,10,0,w,0,h);
    // arg.renderer.ApplyViewingVolume(volume);
    // //glBindTexture(GL_TEXTURE_2D, fb->GetDepthTexture()->GetID());
    // glBindTexture(GL_TEXTURE_2D, depthTextureId);
    // glActiveTextureARB(GL_TEXTURE0);
    // glEnable(GL_TEXTURE_2D);

    // glColor4f(1,1,1,1);
    // glDisable(GL_LIGHTING);
    // glDisable(GL_COLOR_MATERIAL);
    

    // glTranslated(0,0,1);    

    // Quad(w/2,h/2);
    
    // glBindTexture(GL_TEXTURE_2D, 0);
    // glDisable(GL_TEXTURE_2D);

    // return ;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2,w/2,
            -h/2,h/2,
            1,20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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


    glActiveTextureARB(GL_TEXTURE0);
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
    ValueList values = Inspect(light->lightCam);
    ValueList perspI = Inspect(light->lightPersp);
    values.merge(perspI);

    /* Shadows Enabled */ {
        RWValueCall<ShadowMap, bool> *v
            = new RWValueCall<ShadowMap, bool>(*this,
                                               &ShadowMap::GetShadowsEnabled,
                                               &ShadowMap::SetShadowsEnabled);
        v->name = "shadows";
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
    /* Sky Color */ {
        RWValueCall<ShadowMap, Vector<3,float> > *v
            = new RWValueCall<ShadowMap, Vector<3,float> >
            (*this,
             &ShadowMap::GetSkyColor,
             &ShadowMap::SetSkyColor);
        v->name = "skycolor";
        v->isColor = true;
        values.push_back(v);
    }
    /* Ground Color */ {
        RWValueCall<ShadowMap, Vector<3,float> > *v
            = new RWValueCall<ShadowMap, Vector<3,float> >
            (*this,
             &ShadowMap::GetGroundColor,
             &ShadowMap::SetGroundColor);
        v->name = "groundcolor";
        v->isColor = true;
        values.push_back(v);
    }

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
