#ifndef CLIENTSTATECHANGELISTENER_H
#define CLIENTSTATECHANGELISTENER_H

#include <string>

class ClientStateChangeListener
{
public:
    virtual void OnClientStateChange() = 0;
};

#endif // DEVICELISTENER_H
