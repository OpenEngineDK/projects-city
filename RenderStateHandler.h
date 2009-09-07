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
            case KEY_z: rNode->ToggleOption(RenderStateNode::WIREFRAME);   break;
            case KEY_x: rNode->ToggleOption(RenderStateNode::TEXTURE);     break;
            case KEY_c: rNode->ToggleOption(RenderStateNode::SOFT_NORMAL);      break;
            case KEY_v: rNode->ToggleOption(RenderStateNode::SHADER);      break;
            case KEY_b: rNode->ToggleOption(RenderStateNode::BACKFACE);    break;
            case KEY_n: rNode->ToggleOption(RenderStateNode::HARD_NORMAL);  break;
            case KEY_m: rNode->ToggleOption(RenderStateNode::BINORMAL);    break;
            case KEY_e: rNode->ToggleOption(RenderStateNode::TANGENT);     break;
            case KEY_r: rNode->ToggleOption(RenderStateNode::LIGHTING);     break;
            case KEY_t: rNode->ToggleOption(RenderStateNode::DEPTH_TEST);     break;
            case KEY_y: rNode->ToggleOption(RenderStateNode::COLOR_MATERIAL);     break;
            default: break;
        }
    }
};

#endif
