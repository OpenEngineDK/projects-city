#ifndef _CITY_MAIN_UI_H_
#define _CITY_MAIN_UI_H_

#include <QtGUI>

#include <Display/QtEnvironment.h>
#include <Utils/SimpleSetup.h>
#include <Network/IRCClient.h>

namespace Ui { class MainUI; }

using namespace OpenEngine::Display;
using namespace OpenEngine::Network;
using namespace OpenEngine::Utils;



class MainUI : public QMainWindow, public IListener<JoinedChannelArg> {
    Q_OBJECT;
    Ui::MainUI* ui;
    IRCClient& client;
    IRCChannel* chan;

public slots:
    void SendMsg();
    void TextChanges(QString&);

public:
    MainUI(QtEnvironment& env,
           SimpleSetup& setup,
           IRCClient& client);

    void Handle(JoinedChannelArg arg);
};


#endif
