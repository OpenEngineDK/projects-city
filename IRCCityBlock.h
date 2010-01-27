#ifndef _CITY_IRC_CITY_BLOCK_H_
#define _CITY_IRC_CITY_BLOCK_H_

#include <Scene/ISceneNode.h>
#include <Network/IRCChannel.h>
#include <Renderers/TextureLoader.h>
#include <Resources/CairoResource.h>


#include "IRCCityBuilding.h"

using namespace OpenEngine::Scene;
using namespace OpenEngine::Network;
using namespace OpenEngine::Resources;

using namespace OpenEngine::Renderers;

class IRCCity;

/**
 * Short description.
 *
 * @class IRCCityBlock IRCCityBlock.h ts/city/IRCCityBlock.h
 */
class IRCCityBlock : public IListener<UserJoinedArg>,
                     public IListener<UserPartedArg>,
                     public IListener<MessageEventArg> {
private:
    
    
    ISceneNode* root;

    
    
    
    //map<User*,pair<ISceneNode*,CairoResourcePtr> > users;
    map<User*,IRCCityBuilding*> users;

protected:
    friend class IRCCity;
    friend class IRCCityBuilding;
    IRCCityBlock(IRCCity* c, IRCChannel* ,ISceneNode*);
    IRCChannel* channel;
public:
    
    TextureLoader& loader;
    IRCCity* city;


    void Handle(UserJoinedArg arg);
    void Handle(UserPartedArg arg);
    void Handle(MessageEventArg arg);
};

#endif
