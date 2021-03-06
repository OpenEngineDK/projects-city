#include "IRCCityBuilding.h"
#include "IRCCityBlock.h"
#include <Geometry/FaceBuilder.h>
#include <Scene/TransformationNode.h>
#include <Scene/BlendingNode.h>
#include <Utils/CairoTextTool.h>
#include <Logging/Logger.h>
#include <Resources/IFontTextureResource.h>

#include "IRCCity.h"

using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;


IRCCityBuilding::IRCCityBuilding(IRCCityBlock* block,User* u,ISceneNode* n)
    : user(u),channel(block->channel),root(n),loader(block->loader) {
    random.SeedWithTime();
    font = block->city->font;



    //textRes = CairoResource::Create(64,128);
    font->SetColor(Vector<3,float>(1.0,0.0,1.0));
    textRes = font->CreateFontTexture(128,64);
    textRes->Clear(Vector<4,float>(1,1,1,1));
    
    text = user->nick;

    //CairoTextTool textTool;
    //textTool.DrawText(text, textRes);
    loader.Load(textRes, TextureLoader::RELOAD_IMMEDIATE);
    font->RenderText(text, textRes, random.UniformFloat(0,1)*textRes->GetWidth(), random.UniformFloat(0,1)*textRes->GetHeight());


    //textRes->RebindTexture();

    FaceSet *fs = new FaceSet();
    FaceBuilder::FaceState st;
    st.mat = MaterialPtr(new Material());
    st.mat->AddTexture(textRes);
    st.color = Vector<4,float>(1,0,0,1);

    list<FaceBuilder::FaceState> states;
    states.push_back(st);

    st.color = Vector<4,float>(0,1,0,1);
    states.push_back(st);

    st.color = Vector<4,float>(0,1,1,1);
    states.push_back(st);


    st.color = Vector<4,float>(1,0,1,1);
    states.push_back(st);


    st.color = Vector<4,float>(1,1,0,1);
    states.push_back(st);

    st.color = Vector<4,float>(1,1,1,1);
    states.push_back(st);



    FaceBuilder::MakeABox(fs,
                          states,
                          Vector<3,float>(0,0,0),
                          Vector<3,float>(128,60,64));

    GeometryNode* gn = new GeometryNode(fs);
    // BlendingNode* bn = new BlendingNode();

    // bn->SetSource(BlendingNode::SRC_ALPHA);
    // bn->SetDestination(BlendingNode::ONE_MINUS_SRC_ALPHA);

    // bn->AddNode(gn);

    root->AddNode(gn);
}

void IRCCityBuilding::AppendMsg(string m) {

    // text += "\r\n";
    // text += m;

    text = m;
    font->RenderText(text, textRes, random.UniformFloat(0,1)*textRes->GetWidth(), random.UniformFloat(0,1)*textRes->GetHeight());

    // CairoTextTool textTool;
    // textTool.DrawText(text, textRes);
    // textRes->RebindTexture();

}
