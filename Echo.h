#ifndef _CITY_ECHO_H_
#define _CITY_ECHO_H_

#include <Network/TCPSocket.h>
#include <Core/EngineEvents.h>
#include <Core/IListener.h>
#include <Core/Thread.h>
#include <Core/LockedQueuedEvent.h>
#include <Logging/Logger.h>

using namespace OpenEngine::Network;
using namespace OpenEngine::Core;

struct LineEventArg {
    string line;

    LineEventArg(string l) : line(l) {}
};

/**
 * Short description.
 *
 * @class Echo Echo.h ts/city/Echo.h
 */
class Echo : public Thread, public IListener<OpenEngine::Core::ProcessEventArg> {
private:
    TCPSocket* sock;
    bool run;
    LockedQueuedEvent<LineEventArg> lineEvent;
    
public:

    Echo(string host, int port);
    ~Echo();

    void Handle(OpenEngine::Core::ProcessEventArg arg);
    void Run();
    void Stop();

    IEvent<LineEventArg>& LineEvent() {
        return lineEvent;
    }
};

class EchoPrint : public IListener<LineEventArg> {

public:
    void Handle(LineEventArg arg) {
        logger.info << "EchoPrint: " << arg.line << logger.end;
    }

};

#endif
