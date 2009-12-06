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
#include <Geometry/FaceBuilder.h>
#include "RenderStateHandler.h"
#include "City.h"
#include "CityAnimator.h"

#include <Utils/FPSSurface.h>
#include <Display/InterpolatedViewingVolume.h>
#include <Display/PerspectiveViewingVolume.h>
#include <Display/TrackingCamera.h>
#include <Display/QtEnvironment.h>
#include <Resources/SDLFont.h>
#include <Resources/ResourceManager.h>

//#include <Scene/VertexArrayTransformer.h>
#include <Script/Scheme.h>
#include <Script/ScriptBridge.h>
#include <Network/IRCClient.h>

// Game factory
#include "Echo.h"
#include "MainUI.h"
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

/**
 * Main method for the first quarter project of CGD.
 * Corresponds to the
 *   public static void main(String args[])
 * method in Java.
 */
int main(int argc, char** argv) {

    QtEnvironment *env = new QtEnvironment(false);

    SimpleSetup* setup = new SimpleSetup("Example Project Title",NULL,env,NULL);
    setup->AddDataDirectory("projects/city/data/");

    // Print usage info.
    logger.info << "========= Running OpenEngine Test Project =========" << logger.end;

    // make something to display.

    //PerspectiveViewingVolume *persp = new PerspectiveViewingVolume();
    //persp->SetFOV(PI/1.1);

    //TrackingCamera *trackingCam = new TrackingCamera(*(new PerspectiveViewingVolume));

    //setup->SetCamera(*trackingCam);

    //City* c = new City();
    //Scheme *s = new Scheme();
    // s->AddFileToAutoLoad("init.scm");
    // s->AddFileToAutoLoad("oe-init.scm");
    // s->AddFileToAutoLoad("test.scm");
    //s->EvalAndPrint("(display (+ 1 2))");
    //setup->GetEngine().ProcessEvent().Attach(*s);


    //ISceneNode* node = c->GetNode();

    ResourceManager<IFontResource>::AddPlugin(new SDLFontPlugin());

    RenderStateNode *rsn = new RenderStateNode();

    rsn->EnableOption(RenderStateNode::TEXTURE);
    rsn->EnableOption(RenderStateNode::COLOR_MATERIAL);

    TransformationNode *lightTrans = new TransformationNode();
    lightTrans->Move(0, 50, 50);
    //lightTrans->Rotate(0, 0, 0);
    LightNode *ln = new PointLightNode();
    ln->diffuse = Vector<4,float>(.5,.5,.5,1);
    ISceneNode *root = rsn;
    //root = new SceneNode();
    

    //ln->ambient = Vector<4,float>(1,1,1,1);
    lightTrans->AddNode(ln);
    root->AddNode(lightTrans);

    //root->AddNode(node);
    setup->GetScene()->RemoveAllNodes();
    setup->GetScene()->AddNode(root);

    // Transform the scene to use vertex arrays
    // VertexArrayTransformer vaT;
    // vaT.Transform(*(setup->GetScene()));


    // setup->GetCamera()->SetPosition(Vector<3,float>(0,500,0));
    // setup->GetCamera()->LookAt(0, 0, 0);
    //setup->GetCamera()->SetPosition(Vector<3,float>(10,10,10));

    // DAS DOT!

    //Vector<2,float> cross = c->GetGrid()->PositionForCrossing(0,0);

    // TransformationNode* dotTrans = new TransformationNode();
    // dotTrans->SetPosition(Vector<3,float>(cross[0],0,cross[1]));

    // FaceBuilder::FaceState st;
    // st.color=Vector<4,float>(1,1,0,1);
    // FaceSet* fs = new FaceSet();
    // FaceBuilder::MakeABox(fs, st,
    //                       Vector<3,float>(),
    //                       Vector<3,float>(10,10,10));
    // GeometryNode* dot = new GeometryNode(fs);
    // dotTrans->AddNode(dot);
    // trackingCam->Follow(dotTrans);

    // ScriptBridge::AddHandler<TransformationNode>(new TransformationNodeHandler());
    // ScriptBridge::AddHandler<Vector<3,float> >(new VectorHandler());

//     sbo sb = ScriptBridge::CreateSboPointer<TransformationNode>(dotTrans);

//     s->DefineSbo("dot-tn",sb);

// //    CityAnimator *an = new CityAnimator(c,setup->GetCamera(),dotTrans);
// //    setup->GetEngine().ProcessEvent().Attach(*an);
//     setup->GetScene()->AddNode(dotTrans);

    // handlers
    MoveHandler *move = new MoveHandler(*setup->GetCamera(),
                                        setup->GetMouse());

    //move->nodes.push_back(lightTrans);
    // Start the engine.
    setup->GetEngine().InitializeEvent().Attach(*move);
    //setup->GetEngine().ProcessEvent().Attach(*move);
    setup->GetKeyboard().KeyEvent().Attach(*move);
    setup->GetKeyboard().KeyEvent().Attach(*(new RenderStateHandler(rsn)));
    setup->GetJoystick().JoystickAxisEvent().Attach(*move);

    setup->ShowFPS();


    setup->GetRenderer().SetBackgroundColor(Vector<4,float>(.5,.5,.5,1));


    setup->GetCamera()->SetPosition(Vector<3,float>(0,200,-200));
    setup->GetCamera()->LookAt(0,0,0);

    // irc client
    string nick(getlogin());
    nick += "-oe";

    //IRCClient* client = new IRCClient("irc.freenode.org","oe-test","oeeer");
    IRCClient* client = new IRCClient("irc.irczone.dk",nick,"oeeer");
    client->Join("#openengine");

    setup->GetEngine().ProcessEvent().Attach(*client);

    SceneNode* ircNode = new SceneNode();

    IRCCity* ic = new IRCCity(*client, ircNode, setup->GetTextureLoader());

    
    // IFontResourcePtr font = ResourceManager<IFontResource>::Create("Fonts/FreeSerif.ttf");
    // font->SetPointSize(12.0);
    // font->SetFontColor(Vector<3,float>(1,0,0));
    // font->Load();
    // ic->font = font;

    setup->GetScene()->AddNode(ircNode);

    MainUI* ui = new MainUI(*env, *setup, *client);

    client->Start();

    setup->GetEngine().Start();

    client->Stop();
    client->Wait();

    // Return when the engine stops.
    return EXIT_SUCCESS;
}


