#ifndef _CITY_ECHO_H_
#define _CITY_ECHO_H_

#include <Network/TCPSocket.h>
#include <Core/EngineEvents.h>
#include <Core/IListener.h>

using namespace OpenEngine::Network;
using namespace OpenEngine::Core;

/**
 * Short description.
 *
 * @class Echo Echo.h ts/city/Echo.h
 */
class Echo : public IListener<ProcessEventArg> {
private:
    TCPSocket* sock;
public:
    Echo(string host, int port);

    void Handle(ProcessEventArg arg);
};

#endif
