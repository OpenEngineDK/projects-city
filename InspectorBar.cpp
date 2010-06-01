#include "InspectorBar.h"
#include <Logging/Logger.h>

#include <Display/AntTweakBar.h>

using namespace OpenEngine::Display;

InspectorBar::InspectorBar(ValueList vl) 
  : ITweakBar("test"),
    values(vl)
{
    
}

void InspectorBar::AddFields(AntTweakBar& m) {

    
    TwBar* twBar = GetBar();    
    for (ValueList::iterator itr = values.begin();
         itr != values.end();
         itr++) {

        IValue* val = *itr;
               
        if (RWValue<Quaternion<float> > *qv = dynamic_cast<RWValue<Quaternion<float> >* >(val)) {
            Callback<RWValue<Quaternion<float> > > *cb
                = new Callback<RWValue<Quaternion<float> > >(*this,
                                                                &InspectorBar::GetQuaternion,
                                                                &InspectorBar::SetQuaternion,
                                                                qv);

            TwAddVarCB(twBar, 
                       qv->name.c_str(),
                       TW_TYPE_QUAT4F,
                       &InspectorBar::AntSetCallback,
                       &InspectorBar::AntGetCallback,
                       cb,
                       "");
        } else if (RWValue<Vector<3,float> > *vv = dynamic_cast<RWValue<Vector<3,float> >* > (val)) {

            Callback<RWValue<Vector<3,float> > > *cb
                = new Callback<RWValue<Vector<3, float> > >(*this,
                                                            &InspectorBar::GetVector3f,
                                                            &InspectorBar::SetVector3f,
                                                            vv);


            TwAddVarCB(twBar, 
                       vv->name.c_str(),
                       m.antVec3fType,
                       &InspectorBar::AntSetCallback,
                       &InspectorBar::AntGetCallback,
                       cb,
                       "");

            
        } else {

            string* hesten = new string("hesten"); 
            TwAddVarRO(twBar, val->name.c_str(), TW_TYPE_STDSTRING, hesten, "");
        }
    }
}

void InspectorBar::GetQuaternion(void *value, RWValue<Quaternion<float> > *rwv) {
    Quaternion<float> q = rwv->Get();
    float *val = (float*)value;
    val[0] = q[0];
    val[1] = q[1];
    val[2] = q[2];
    val[3] = q[3];
}

void InspectorBar::SetQuaternion(const void *value, RWValue<Quaternion<float> > *rwv) {
    Quaternion<float> q;
    float *val = (float*)value;
    q[0] = val[0];
    q[1] = val[1];
    q[2] = val[2];
    q[3] = val[3];
    rwv->Set(q);
}

void InspectorBar::GetVector3f(void *value, RWValue<Vector<3,float> > *rwv) {
    Vector<3,float> v = rwv->Get();
    AntTweakBar::antVec3f *val = (AntTweakBar::antVec3f*)value;
    val->x = v[0];
    val->y = v[1];
    val->z = v[2];
}

void InspectorBar::SetVector3f(const void *value, RWValue<Vector<3,float> > *rwv) {
    Vector<3,float> v;
    AntTweakBar::antVec3f *val = (AntTweakBar::antVec3f*)value;
    v[0] = val->x;
    v[1] = val->y;
    v[2] = val->z;
    rwv->Set(v);
}


void TW_CALL InspectorBar::AntSetCallback(const void *value, void *clientdata) {
    ICallback* cb = (ICallback*)clientdata;
    cb->Set(value);
}
void TW_CALL InspectorBar::AntGetCallback(void *value, void *clientdata) {
    ICallback* cb = (ICallback*)clientdata;
    cb->Get(value);
}
