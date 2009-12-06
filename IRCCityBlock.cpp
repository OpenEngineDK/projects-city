#include "IRCCityBlock.h"
#include <Geometry/FaceBuilder.h>
#include <Scene/TransformationNode.h>
#include <Resources/IFontResource.h>
#include <Utils/CairoTextTool.h>

#include "IRCCity.h"

using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;



IRCCityBlock::IRCCityBlock(IRCCity* ci, IRCChannel* c,ISceneNode* n)
    : root(n),channel(c),loader(ci->loader), city(ci)
{
    channel->UserJoinedEvent().Attach(*this);
    channel->UserPartedEvent().Attach(*this);
    channel->ChannelMessageEvent().Attach(*this);
}

void IRCCityBlock::Handle(UserJoinedArg arg) {

    TransformationNode* node = new TransformationNode();

    node->Move(0,78*users.size(),0);
    node->SetScale(Vector<3,float>(1.0,-1.0,1.0));

    users.insert(make_pair<>(arg.user, new IRCCityBuilding(this,arg.user, node)));
    
    root->AddNode(node);
}

void IRCCityBlock::Handle(UserPartedArg arg) {

}

void IRCCityBlock::Handle(MessageEventArg arg) {
    IRCCityBuilding* b = users[arg.user];
    if (!b)
        throw "building not found";

    b->AppendMsg(arg.message);
}
