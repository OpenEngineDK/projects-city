// main
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------

// OpenEngine stuff
#include <Meta/Config.h>
#include <Logging/Logger.h>
#include <Logging/StreamLogger.h>
#include <Core/Engine.h>
#include <Geometry/FaceSet.h>
#include <Geometry/Face.h>
#include <Scene/RenderStateNode.h>
#include <Scene/PointLightNode.h>
#include <Scene/SceneNode.h>

// SimpleSetup
#include <Utils/SimpleSetup.h>
#include <Utils/MoveHandler.h>
#include <Utils/BetterMoveHandler.h>
#include <Geometry/FaceBuilder.h>

#include <Utils/RenderStateHandler.h>

#include "City.h"
#include "CityAnimator.h"
#include <Utils/InspectionBar.h>

#include <Utils/InspectionBar.h>

//#include <Utils/FPSSurface.h>
#include <Display/InterpolatedViewingVolume.h>
#include <Display/PerspectiveViewingVolume.h>
#include <Display/TrackingCamera.h>
#include <Display/SDLEnvironment.h>

#include <Display/AntTweakBar.h>
#include <Display/TestBar.h>

// #include <Resources/SDLFont.h>
#include <Resources/CairoFont.h>
#include <Resources/ResourceManager.h>

#include <Scene/VertexArrayTransformer.h>
#include <Script/Scheme.h>
#include <Script/ScriptBridge.h>
#include <Network/IRCClient.h>

#include <Utils/CameraTool.h>
#include <Utils/ToolChain.h>
#include <Utils/MouseSelection.h>

#include <Utils/RenderStateNodeInspector.h>
#include <Utils/TransformationNodeInspector.h>
#include <Utils/CameraInspector.h>
#include <Utils/PerspectiveViewingVolumeInspector.h>

#include "Renderers/OpenGL/AmbientOcclusion.h"
#include "Renderers/OpenGL/ShadowMap.h"
#include <Resources/AssimpResource.h>

// Game factory
#include "Echo.h"
//#include "MainUI.h"
#include "IRCCity.h"

#include "ShadowLight.h"

// name spaces that we will be using.
// this combined with the above imports is almost the same as
// fx. import OpenEngine.Logging.*; in Java.
using namespace OpenEngine::Logging;
using namespace OpenEngine::Core;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Display;

using namespace OpenEngine::Geometry;
using namespace OpenEngine::Resources;

using namespace OpenEngine::Script;

using OpenEngine::Renderers::OpenGL::AmbientOcclusion;
using OpenEngine::Renderers::OpenGL::ShadowMap;

namespace OpenEngine {
    namespace Utils{
        namespace Inspection {

            ValueList Inspect(AmbientOcclusion* ao) {
                ValueList values;
                {
                    RWValueCall<AmbientOcclusion, bool> *v
                        = new RWValueCall<AmbientOcclusion, bool>
                        (*ao,
                         &AmbientOcclusion::IsEnabled,
                         &AmbientOcclusion::SetEnabled);
                    v->name = "enable";
                    values.push_back(v);
                }
                {
                    RWValueCall<AmbientOcclusion, bool> *v
                        = new RWValueCall<AmbientOcclusion, bool>
                        (*ao,
                         &AmbientOcclusion::GetBlur,
                         &AmbientOcclusion::SetBlur);
                    v->name = "blur AO";
                    values.push_back(v);
                }
                {
                    RWValueCall<AmbientOcclusion, bool> *v
                        = new RWValueCall<AmbientOcclusion, bool>
                        (*ao,
                         &AmbientOcclusion::GetMerge,
                         &AmbientOcclusion::SetMerge);
                    v->name = "merge AO";
                    values.push_back(v);
                }
                {
                    RWValueCall<AmbientOcclusion, float> *v
                        = new RWValueCall<AmbientOcclusion, float>
                        (*ao,
                         &AmbientOcclusion::GetRadius,
                         &AmbientOcclusion::SetRadius);
                    v->name = "Radius";
                    v->properties[MIN] = 0;
                    v->properties[MAX] = 100;
                    v->properties[STEP] = 0.1;
                    values.push_back(v);
                }
                {
                    RWValueCall<AmbientOcclusion, float> *v
                        = new RWValueCall<AmbientOcclusion, float>
                        (*ao,
                         &AmbientOcclusion::GetLinearAttenuation,
                         &AmbientOcclusion::SetLinearAttenuation);
                    v->name = "linear attenuation";
                    v->properties[MIN] = 0;
                    v->properties[MAX] = 1;
                    v->properties[STEP] = 0.1;
                    values.push_back(v);
                }
                {
                    RWValueCall<AmbientOcclusion, float> *v
                        = new RWValueCall<AmbientOcclusion, float>
                        (*ao,
                         &AmbientOcclusion::GetContrast,
                         &AmbientOcclusion::SetContrast);
                    v->name = "contrast";
                    v->properties[MIN] = 0;
                    v->properties[MAX] = 1;
                    v->properties[STEP] = 0.1;
                    values.push_back(v);
                }
                {
                    RWValueCall<AmbientOcclusion, float> *v
                        = new RWValueCall<AmbientOcclusion, float>
                        (*ao,
                         &AmbientOcclusion::GetNumOfRays,
                         &AmbientOcclusion::SetNumOfRays);
                    v->name = "Number of rays";
                    v->properties[MIN] = 1;
                    v->properties[MAX] = 64;
                    v->properties[STEP] = 1;
                    values.push_back(v);
                }
                {
                    RWValueCall<AmbientOcclusion, float> *v
                        = new RWValueCall<AmbientOcclusion, float>
                        (*ao,
                         &AmbientOcclusion::GetNumOfSteps,
                         &AmbientOcclusion::SetNumOfSteps);
                    v->name = "Number of steps";
                    v->properties[MIN] = 1;
                    v->properties[MAX] = 50;
                    v->properties[STEP] = 1;
                    values.push_back(v);
                }
                {
                    RWValueCall<AmbientOcclusion, float> *v
                        = new RWValueCall<AmbientOcclusion, float>
                        (*ao,
                         &AmbientOcclusion::GetAngleBias,
                         &AmbientOcclusion::SetAngleBias);
                    v->name = "angle bias";
                    v->properties[MIN] = 0;
                    v->properties[MAX] = Math::PI;
                    v->properties[STEP] = Math::PI/10.0;
                    values.push_back(v);
                }
                return values;    
            }
        }
    }
}

/**
 * Main method for the first quarter project of CGD.
 * Corresponds to the
 *   public static void main(String args[])
 * method in Java.
 */
int main(int argc, char** argv) {
    //SDLEnvironment *env = new SDLEnvironment(800, 600);
    SDLEnvironment *env = new SDLEnvironment(1024, 768);

    PerspectiveViewingVolume *persp = new PerspectiveViewingVolume(10,100);
    Camera* lc = new Camera(*persp);

    TransformationNode *lightTrans = new TransformationNode();
    lightTrans->SetPosition(Vector<3,float>(100, 100, 100));
    //lightTrans->Rotate(0, 0, 0);
    PointLightNode *ln = new PointLightNode();
    ln->diffuse = Vector<4,float>(.5,.5,.5,1);
    lightTrans->AddNode(ln);


    ShadowLight *sl = new ShadowLight(lc, persp,lightTrans);   
    ShadowMap* sm = new ShadowMap(sl);

   
    SimpleSetup* setup = new SimpleSetup("Example Project Title",env,sm); // With Shadowmap
    //SimpleSetup* setup = new SimpleSetup("Example Project Title",env); // without
    setup->AddDataDirectory("projects/city/data/");

    // Print usage info.
    logger.info << "========= Hello World =========" << logger.end;

    City* c = new City();


    ISceneNode* node = c->GetNode();

    ResourceManager<IFontResource>::AddPlugin(new CairoFontPlugin());
    ResourceManager<IModelResource>::AddPlugin(new AssimpPlugin());

    RenderStateNode *rsn = new RenderStateNode();

    rsn->EnableOption(RenderStateNode::LIGHTING);
    rsn->DisableOption(RenderStateNode::COLOR_MATERIAL);
    rsn->EnableOption(RenderStateNode::SHADER);
    //rsn->DisableOption(RenderStateNode::LIGHTING);
    //rsn->DisableOption(RenderStateNode::BACKFACE);
    ISceneNode *root = rsn;

    root->AddNode(lightTrans);
    //root->AddNode(sl);


    
    
    TransformationNode* duckTrans = new TransformationNode();
    duckTrans->SetPosition(Vector<3,float>(20, 0, -20));
    duckTrans->SetScale(Vector<3,float>(50.0));
    
    //IModelResourcePtr duckRes = ResourceManager<IModelResource>::Create("bunny/bun_zipper.ply");
    //IModelResourcePtr duckRes = ResourceManager<IModelResource>::Create("Collada/duck.dae");
    IModelResourcePtr duckRes = ResourceManager<IModelResource>::Create("dragon/dragon_vrip_res3.ply");
    duckRes->Load();
    ISceneNode* duck = duckRes->GetSceneNode();
    duckRes->Unload();
    duckTrans->AddNode(duck);
    root->AddNode(duckTrans);
    //ln->ambient = Vector<4,float>(1,1,1,1);

    //root->AddNode(node); // Add the city
    setup->GetScene()->RemoveAllNodes();
    setup->GetScene()->AddNode(root);


    TransformationNode* thingyTrans = new TransformationNode();
    IModelResourcePtr thingyRes = ResourceManager<IModelResource>::Create("thingy/Hesty.dae");
    thingyRes->Load();
    thingyTrans->AddNode(thingyRes->GetSceneNode());
    thingyRes->Unload();
    root->AddNode(thingyTrans);
    thingyTrans->SetRotation(Quaternion<float>(-PI/3.0, Vector<3,float>(1,0,0)));



    //setup->ShowFPS();

    setup->GetRenderer().SetBackgroundColor(Vector<4,float>(.4,.4,.4,1));
    AmbientOcclusion* ao = new AmbientOcclusion();

    
    ao->AttachTo(setup->GetRenderer());

    

    // ant tweak bar
    AntTweakBar *atb = new AntTweakBar();
    atb->AttachTo(setup->GetRenderer());

    setup->GetKeyboard().KeyEvent().Attach(*atb);
    setup->GetMouse().MouseMovedEvent().Attach(*atb);
    setup->GetMouse().MouseButtonEvent().Attach(*atb);
    

    // handlers
    BetterMoveHandler *move = new BetterMoveHandler(*setup->GetCamera(),
                                                    setup->GetMouse(),
                                                    true);
    // move->nodes.push_back(lightTrans);

    setup->GetEngine().InitializeEvent().Attach(*move);
    setup->GetEngine().ProcessEvent().Attach(*move);
    setup->GetJoystick().JoystickAxisEvent().Attach(*move);
    atb->KeyEvent().Attach(*move);   
    atb->MouseButtonEvent().Attach(*move);
    atb->MouseMovedEvent().Attach(*move);

    atb->AddBar(new InspectionBar("model",Inspect(duckTrans)));     
    atb->AddBar(new InspectionBar("ambient occlusion", Inspect(ao)));

    ValueList shadowI = Inspect(lc);
    ValueList perspI = Inspect(persp);
    shadowI.merge(perspI);
    

    atb->AddBar(new InspectionBar("thingy",Inspect(thingyTrans)));
    atb->AddBar(new InspectionBar("shadow",Inspect(sm)));
    atb->AddBar(new InspectionBar("renderer",Inspect(rsn)));

    setup->GetKeyboard().KeyEvent().Attach(*(new RenderStateHandler(*rsn)));

    setup->GetCamera()->SetPosition(Vector<3,float>(20,20,20));
    setup->GetCamera()->LookAt(0,0,0);

    lc->SetPosition(Vector<3,float>(-10,20,10));
    lc->LookAt(20,0,-20);

    move->PushCamera(lc);

    setup->GetEngine().Start();

    // Return when the engine stops.
    return EXIT_SUCCESS;
}


