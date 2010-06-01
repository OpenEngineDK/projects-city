// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_INSPECTOR_BAR_H_
#define _OE_INSPECTOR_BAR_H_

#include <Display/ITweakBar.h>

#include <Utils/IInspector.h>

#include <list>
#include <Math/Quaternion.h>

using namespace OpenEngine::Math;

using namespace OpenEngine::Display;
using namespace OpenEngine::Utils;

/**
 * Short description.
 *
 * @class InspectorBar InspectorBar.h ts/city/InspectorBar.h
 */
class InspectorBar : public ITweakBar {
private:

    class ICallback {
    public:
        virtual void Get(void *) =0;
        virtual void Set(const void *) =0;
    };

    template <class T>
    class Callback : public ICallback {
        InspectorBar& instance;
        void (InspectorBar::*getFun)(void*, T*);
        void (InspectorBar::*setFun)(const void*, T*);
        T* arg;
    public:
        Callback(InspectorBar& ins,
                 void (InspectorBar::*gFun)(void*, T*),
                 void (InspectorBar::*sFun)(const void*, T*),
                 T* arg)
            : instance(ins),
              getFun(gFun),
              setFun(sFun),
              arg(arg) {}
        void Get(void *v) {
            (instance.*getFun)(v,arg);
        }
        void Set(const void *v) {
            (instance.*setFun)(v,arg);
        }
    };
    


    ValueList values;
    void GetQuaternion(void *value, RWValue<Quaternion<float> >* rwv);
    void SetQuaternion(const void *value, RWValue<Quaternion<float> >* rwv);
    void GetVector3f(void *value, RWValue<Vector<3,float> >* rwv);
    void SetVector3f(const void *value, RWValue<Vector<3,float> >* rwv);
    
public:
    InspectorBar(string title, ValueList vl);

    void AddFields(AntTweakBar& m);
    static void TW_CALL AntSetCallback(const void *value, void *clientdata);
    static void TW_CALL AntGetCallback(void *value, void *clientdata);
};

#endif // _OE_INSPECTOR_BAR_H_
