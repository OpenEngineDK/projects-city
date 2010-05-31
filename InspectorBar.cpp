#include "InspectorBar.h"
#include <Logging/Logger.h>


InspectorBar::InspectorBar(ValueList vl) 
  : ITweakBar("test"),
    values(vl)
{
    
}


void InspectorBar::AddFields(ITweakBar* b) {
    TwBar* twBar = GetBar();    
    for (ValueList::iterator itr = values.begin();
         itr != values.end();
         itr++) {

        IValue* val = *itr;
        
        RWValue<Quaternion<float> > *qv = dynamic_cast<RWValue<Quaternion<float> >* >(val);
        if (qv) {
            Callback<RWValue<Quaternion<float> > > *cb
                = new Callback<RWValue<Quaternion<float> > >(*this,
                                                                &InspectorBar::GetQuaternion,
                                                                &InspectorBar::SetQuaternion,
                                                                qv);

            TwAddVarCB(twBar, 
                       val->name.c_str(),
                       TW_TYPE_QUAT4F,
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

void TW_CALL InspectorBar::AntSetCallback(const void *value, void *clientdata) {
    ICallback* cb = (ICallback*)clientdata;
    cb->Set(value);
}
void TW_CALL InspectorBar::AntGetCallback(void *value, void *clientdata) {
    ICallback* cb = (ICallback*)clientdata;
    cb->Get(value);
}
