#include "IRCCity.h"

#include <Logging/Logger.h>
#include <Geometry/FaceBuilder.h>
#include <Scene/TransformationNode.h>

using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;

IRCCity::IRCCity(IRCClient& c, ISceneNode* n, TextureLoader& l) : client(c),root(n), loader(l) {
    c.JoinedChannelEvent().Attach(*this);
}

void IRCCity::Handle(JoinedChannelArg arg) {
    // Create a city
    logger.info << "Create a new city" << logger.end;

    TransformationNode* cityNode = new TransformationNode();
    root->AddNode(cityNode);

    channels.push_back(new IRCCityBlock(this, arg.channel, cityNode));

    // FaceSet* fs = new FaceSet();
    // FaceBuilder::FaceState st;
    // st.color = Vector<4,float>(1,0,0,1);

    // FaceBuilder::MakeABox(fs, 
    //                       st,
    //                       Vector<3,float>(0,0,0),
    //                       Vector<3,float>(10,10,10));

    // GeometryNode *gn = new GeometryNode(fs);
 

    // root->AddNode(gn);
}
