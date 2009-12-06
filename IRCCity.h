#ifndef _CITY_IRC_CITY_H_
#define _CITY_IRC_CITY_H_

#include <Core/IListener.h>
#include <Network/IRCClient.h>
#include <Scene/ISceneNode.h>

#include "IRCCityBlock.h"

using namespace OpenEngine::Network;
using namespace OpenEngine::Scene;


/**
 * Short description.
 *
 * @class IRCCity IRCCity.h ts/city/IRCCity.h
 */
class IRCCity :  public IListener<JoinedChannelArg> {
private:
    IRCClient& client;
    ISceneNode* root;
    list<IRCCityBlock*> channels;
public:

    IFontResourcePtr font;

    TextureLoader& loader;    

    IRCCity(IRCClient&, ISceneNode*, TextureLoader&);
    

    void Handle(JoinedChannelArg arg);
};

#endif
