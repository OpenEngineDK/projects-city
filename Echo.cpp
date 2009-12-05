#include "Echo.h"

Echo::Echo(string host, int port) {
    sock = new TCPSocket(port);
    sock->Connect(host);
}


void Echo::Handle(ProcessEventArg arg) {
    
}
