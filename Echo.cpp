#include "Echo.h"


Echo::Echo(string host, int port) : run(true) {
    sock = new TCPSocket(port);
    sock->Connect(host);
}

Echo::~Echo() {
    delete sock;
}

void Echo::Handle(ProcessEventArg arg) {
    lineEvent.Release();
}

void Echo::Stop() {
    run = false;
    sock->Close();
}

void Echo::Run() {
    while (run && sock->IsOpen()) {
        string l = sock->ReadLine();
        
        lineEvent.Notify(LineEventArg(l));

    }
}
