#ifndef SERVERINTERACTOR_H
#define SERVERINTERACTOR_H

#include <boost/asio.hpp>
#include <string>
#include "deviceconfigurationstore.h"
#include "statechangelistener.h"
#include "udpserver.h"


class MessageHandler;

class ServerPresenter
{
public:
    virtual void PresentServerState(std::string ServerState) = 0;
};

class ServerConfigurationStore
{
public:
    virtual void StoreServerConfiguration(const DeviceConfiguration& Configuration) = 0;
    virtual DeviceConfiguration GetServerConfiguration() = 0;
};

class ServerInteractor : public StateChangeListener<UdpServerState>
{
public:
    ServerInteractor(ServerPresenter& Presenter,
                     ServerConfigurationStore& Store);

    ~ServerInteractor();

    void InitFromStore(boost::asio::io_service& IOService, MessageHandler& MessageHandler);
    void StartServer(const DeviceConfiguration& Configuration,
                     boost::asio::io_service& IOService,
                     MessageHandler& MessageHandler);

    void StopServer();

    void OnStateChange() override;

private:
    UdpServer* m_ServerDevice = nullptr;
    ServerPresenter& m_Presenter;
    ServerConfigurationStore& m_ConfigurationStore;
};

#endif // SERVERINTERACTOR_H
