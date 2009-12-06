#include "IRCCityBuilding.h"
#include "IRCCityBlock.h"
#include <Geometry/FaceBuilder.h>
#include <Scene/TransformationNode.h>
#include <Utils/CairoTextTool.h>
#include <Logging/Logger.h>

using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;


IRCCityBuilding::IRCCityBuilding(IRCCityBlock* block,User* u,ISceneNode* n) 
    : user(u),channel(block->channel),root(n),loader(block->loader) {

    textRes = CairoResource::Create(64,128);
    textRes->Load();

    text = user->nick;

    CairoTextTool textTool;
    textTool.DrawText(text, textRes);

    loader.Load(textRes, TextureLoader::RELOAD_QUEUED);
    textRes->RebindTexture();

    FaceSet *fs = new FaceSet();
    FaceBuilder::FaceState st;
    st.mat = MaterialPtr(new Material());
    st.mat->texr = textRes;

    FaceBuilder::MakeABox(fs,
                          st,
                          Vector<3,float>(0,0,0),
                          Vector<3,float>(64,60,128));

    GeometryNode* gn = new GeometryNode(fs);


    root->AddNode(gn);  
}

void IRCCityBuilding::AppendMsg(string m) {
   
    text += "\n";
    text += m;
    
    CairoTextTool textTool;
    textTool.DrawText(text, textRes);
    textRes->RebindTexture();

}
