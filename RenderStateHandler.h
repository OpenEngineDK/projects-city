#ifndef _RENDER_STATE_HANDLER_H_
#define _RENDER_STATE_HANDLER_H_

#include <Devices/Symbols.h>
#include <Devices/IKeyboard.h>
#include <Core/IListener.h>

using namespace OpenEngine::Devices;
using OpenEngine::Core::IListener;

class RenderStateHandler : public IListener<KeyboardEventArg> {
private:
    RenderStateNode* rNode;
public:
    RenderStateHandler(RenderStateNode* rNode) : rNode(rNode) {}
    void Handle(KeyboardEventArg arg) {
        if (arg.type != EVENT_PRESS)
            return;
        if (rNode == NULL) {
            logger.info << "ERROR NO RenderStateNode found!" << logger.end;
            return;
        }
        switch (arg.sym) {
            case KEY_1: rNode->ToggleOption(RenderStateNode::WIREFRAME);   break;
            case KEY_2: rNode->ToggleOption(RenderStateNode::TEXTURE);     break;
            case KEY_3: rNode->ToggleOption(RenderStateNode::SOFT_NORMAL);      break;
            case KEY_4: rNode->ToggleOption(RenderStateNode::SHADER);      break;
            case KEY_5: rNode->ToggleOption(RenderStateNode::BACKFACE);    break;
            case KEY_6: rNode->ToggleOption(RenderStateNode::HARD_NORMAL);  break;
            case KEY_7: rNode->ToggleOption(RenderStateNode::BINORMAL);    break;
            case KEY_8: rNode->ToggleOption(RenderStateNode::TANGENT);     break;
            case KEY_9: rNode->ToggleOption(RenderStateNode::LIGHTING);     break;
            case KEY_0: rNode->ToggleOption(RenderStateNode::DEPTH_TEST);     break;
            default: break;
        }
    }
};

#endif
