#include "serverinteractor.h"
#include "client.h"
#include "udpserver.h"
#include "messagehandler.h"

ServerInteractor::ServerInteractor(ServerPresenter& Presenter,
                                   ServerConfigurationStore& Store) :
    m_Presenter(Presenter),
    m_ConfigurationStore(Store)
{

}


ServerInteractor::~ServerInteractor()
{
   StopServer();
}

void ServerInteractor::InitFromStore(boost::asio::io_service& IOService, MessageHandler& MessageHandler)
{
    DeviceConfiguration StoredConfiguration = m_ConfigurationStore.GetServerConfiguration();
    StartServer(StoredConfiguration, IOService, MessageHandler);
}


void ServerInteractor::StartServer(const DeviceConfiguration& Configuration,
                                   boost::asio::io_service& IOService,
                                   MessageHandler& MessageHandler)
{
    std::string DeviceType;
    if(Configuration.Get("Type", DeviceType) && DeviceType == "UDP_SERVER") // TODO Remove constant
    {
        StopServer();
        m_ServerDevice = new UdpServer(Configuration, IOService, MessageHandler, *this);
        m_ConfigurationStore.StoreServerConfiguration(Configuration);
        m_ServerDevice->Start();
    }
}


void ServerInteractor::StopServer()
{
    if(m_ServerDevice)
    {
        m_ServerDevice->Reset();
        delete m_ServerDevice;
        m_ServerDevice = nullptr;
    }
}

void ServerInteractor::OnStateChange()
{
    auto CurrentDeviceState = m_ServerDevice->GetState();
    switch(CurrentDeviceState)
    {
    case UdpServerState::READY:
        m_Presenter.PresentServerState("Ready");
        break;
    case UdpServerState::ACCEPTING:
        m_Presenter.PresentServerState("Accepting");
        break;
    case UdpServerState::ERROR:
        m_Presenter.PresentServerState("Error");
        break;
     default:
        m_Presenter.PresentServerState("Unknown");
    }
}
