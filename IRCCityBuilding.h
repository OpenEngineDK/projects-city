#ifndef _CITY_IRC_CITY_BUILDING_H_
#define _CITY_IRC_CITY_BUILDING_H_

#include <Network/IRCChannel.h>
#include <Scene/ISceneNode.h>
#include <Renderers/TextureLoader.h>
#include <Resources/CairoResource.h>
#include <Resources/IFontResource.h>
#include <Math/RandomGenerator.h>

using namespace OpenEngine::Network;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Renderers;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Math;


class IRCCityBlock;

/**
 * Short description.
 *
 * @class IRCCityBuilding IRCCityBuilding.h ts/city/IRCCityBuilding.h
 */
class IRCCityBuilding {
private:
        
    User* user;
    IRCChannel* channel;
    ISceneNode* root;

    TextureLoader& loader;
    //CairoResourcePtr textRes;
    IFontTextureResourcePtr textRes;

    IFontResourcePtr font;

    string text;
    RandomGenerator random;

public:
    IRCCityBuilding(IRCCityBlock*,User*,ISceneNode*);
    void AppendMsg(string m);
};

#endif
