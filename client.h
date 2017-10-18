#ifndef CLIENT_H
#define CLIENT_H

#include <thread>
#include "deviceconfiguration.h"
#include "statebearer.h"
#include "clientstate.h"

class Client : public StateBearer<ClientState>
{
public:
    Client(StateChangeListener<ClientState>& Listener);
    virtual void Start() = 0;
    virtual void Reset() = 0;
    virtual bool ReadyToStart() const = 0;
    virtual void Write(const std::string& Message) = 0;
};

#endif // DEVICE_H
