#include "client.h"
#include "clientstate.h"
#include "statechangelistener.h"

Client::Client(StateChangeListener<ClientState>& Listener) :
    StateBearer<ClientState>(Listener, ClientState::READY)
{

}
