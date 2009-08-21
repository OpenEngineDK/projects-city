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
#include "FaceBuilder.h"
#include "RenderStateHandler.h"

// Game factory
//#include "GameFactory.h"

// name spaces that we will be using.
// this combined with the above imports is almost the same as
// fx. import OpenEngine.Logging.*; in Java.
using namespace OpenEngine::Logging;
using namespace OpenEngine::Core;
using namespace OpenEngine::Utils;

using namespace OpenEngine::Geometry;

/**
 * Main method for the first quarter project of CGD.
 * Corresponds to the
 *   public static void main(String args[])
 * method in Java.
 */
int main(int argc, char** argv) {
    // Setup logging facilities.
    Logger::AddLogger(new StreamLogger(&std::cout));

    // Print usage info.
    logger.info << "========= Running OpenEngine Test Project =========" << logger.end;

    // Create simple setup
    SimpleSetup* setup = new SimpleSetup("Example Project Title");
    
    // make something to display.
    
    GeometryNode *node = new GeometryNode(FaceBuilder::MakeABox(10,20,30));
    
    RenderStateNode *rsn = new RenderStateNode();
    
    rsn->EnableOption(RenderStateNode::LIGHTING);
    
    TransformationNode *lightTrans = new TransformationNode();
    lightTrans->Move(0, 50, 50);
    //lightTrans->Rotate(0, 0, 0);
    LightNode *ln = new PointLightNode();
    ln->diffuse = Vector<4,float>(.5,.5,.5,1);
    RenderStateNode *root = rsn;
    
    //ln->ambient = Vector<4,float>(1,1,1,1);
    lightTrans->AddNode(ln);
    root->AddNode(lightTrans);
    
    root->AddNode(node);
    setup->GetScene()->RemoveAllNodes();
    setup->GetScene()->AddNode(root);

    setup->GetCamera()->SetPosition(Vector<3,float>(30,50,40));
    setup->GetCamera()->LookAt(0, 15, 0);

    // handlers
    MoveHandler *move = new MoveHandler(*setup->GetCamera(),
                                        setup->GetMouse());
    
    //move->nodes.push_back(lightTrans);
    // Start the engine.
    setup->GetEngine().InitializeEvent().Attach(*move);
    setup->GetEngine().ProcessEvent().Attach(*move);
    setup->GetKeyboard().KeyEvent().Attach(*move);
    setup->GetKeyboard().KeyEvent().Attach(*(new RenderStateHandler(rsn)));
    
    setup->GetEngine().Start();

    // Return when the engine stops.
    return EXIT_SUCCESS;
}


