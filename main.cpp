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
#include "InspectorBar.h"

#include "InspectorBar.h"


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

#include <Utils/TransformationNodeInspector.h>

#include "Renderers/OpenGL/AmbientOcclusion.h"
#include "Renderers/OpenGL/ShadowMap.h"
#include <Resources/AssimpResource.h>

// Game factory
#include "Echo.h"
//#include "MainUI.h"
#include "IRCCity.h"

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

/**
 * Main method for the first quarter project of CGD.
 * Corresponds to the
 *   public static void main(String args[])
 * method in Java.
 */
int main(int argc, char** argv) {
    SDLEnvironment *env = new SDLEnvironment(800, 600);
   
    SimpleSetup* setup = new SimpleSetup("Example Project Title",env);
    setup->AddDataDirectory("projects/city/data/");

    // Print usage info.
    logger.info << "========= Hello World =========" << logger.end;

    City* c = new City();


    ISceneNode* node = c->GetNode();

    ResourceManager<IFontResource>::AddPlugin(new CairoFontPlugin());
    ResourceManager<IModelResource>::AddPlugin(new AssimpPlugin());

    RenderStateNode *rsn = new RenderStateNode();

    //rsn->EnableOption(RenderStateNode::TEXTURE);
    rsn->EnableOption(RenderStateNode::COLOR_MATERIAL);
    rsn->DisableOption(RenderStateNode::LIGHTING);
    rsn->DisableOption(RenderStateNode::BACKFACE);
    ISceneNode *root = rsn;

    TransformationNode *lightTrans = new TransformationNode();
    lightTrans->SetPosition(Vector<3,float>(100, 100, 100));
    //lightTrans->Rotate(0, 0, 0);
    PointLightNode *ln = new PointLightNode();
    ln->diffuse = Vector<4,float>(.5,.5,.5,1);
    lightTrans->AddNode(ln);
    root->AddNode(lightTrans);
    
    TransformationNode* duckTrans = new TransformationNode();
    duckTrans->SetPosition(Vector<3,float>(400, 0, 400));
    duckTrans->SetScale(Vector<3,float>(1000.0));
    //IModelResourcePtr duckRes = ResourceManager<IModelResource>::Create("bunny/bun_zipper.ply");
    //IModelResourcePtr duckRes = ResourceManager<IModelResource>::Create("Collada/duck.dae");
    IModelResourcePtr duckRes = ResourceManager<IModelResource>::Create("dragon/dragon_vrip_res3.ply");
    duckRes->Load();
    ISceneNode* duck = duckRes->GetSceneNode();
    duckRes->Unload();
    duckTrans->AddNode(duck);
    root->AddNode(duckTrans);
    //ln->ambient = Vector<4,float>(1,1,1,1);

    root->AddNode(node);
    setup->GetScene()->RemoveAllNodes();
    setup->GetScene()->AddNode(root);



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
     
    atb->AddBar(new InspectorBar("duck",Inspect(duckTrans)));


    setup->GetKeyboard().KeyEvent().Attach(*(new RenderStateHandler(*rsn)));


    setup->GetCamera()->SetPosition(Vector<3,float>(0,200,-200));
    setup->GetCamera()->LookAt(0,0,0);

    setup->GetEngine().Start();

    // Return when the engine stops.
    return EXIT_SUCCESS;
}


