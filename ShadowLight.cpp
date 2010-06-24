#include "ShadowLight.h"

ShadowLight::ShadowLight(Camera* lc, PerspectiveViewingVolume* p, TransformationNode* t, Light* l)
    : lightCam(lc),
      lightPersp(p),
      lightTrans(t),
      light(l) {
    
    
      }

void ShadowLight::Update() {
        lightTrans->SetPosition(lightCam->GetPosition());
}
