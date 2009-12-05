#include "MainUI.h"

#include <Logging/Logger.h>
#include <Network/IRCChannel.h>
#include "city/ui_MainUI.h"


MainUI::MainUI(QtEnvironment& env,
               SimpleSetup& setup,
               IRCClient& client) : client(client) {
    ui = new Ui::MainUI();
    ui->setupUi(this);

    ui->topLayout->addWidget(env.GetGLWidget());
    
    client.JoinedChannelEvent().Attach(*this);

    chan = NULL;

    show();
}


void MainUI::Handle(JoinedChannelArg arg) {
    logger.info << "Joined" << logger.end;
    chan = &(arg.channel);
}

void MainUI::SendMsg() {
    
    string str = ui->inputEdit->text().toStdString();

    logger.info << "Send stuff: " << str << logger.end;
    if (str[0] != '/' && chan) {
        
        chan->SendMsg(str);
            
        // Send the string to the channel!

    }
    

    ui->inputEdit->setText(QString());
}
