#include "udpserver.h"
#include "udpserversocketreader.h"
#include <iostream>
#include "messagehandler.h"
#include "deviceconfiguration.h"
#include "statechangelistener.h"

using boost::asio::ip::udp;

UdpServer::UdpServer(unsigned int Port,
                     boost::asio::io_service &IOService,
                     MessageHandler& MessageHandler,
                     StateChangeListener<UdpServerState>& Listener) :
    StateBearer<UdpServerState>(Listener, UdpServerState::READY),
    m_Port(Port),
    m_IOService(IOService),
    m_MessageHandler(MessageHandler)
{

}

UdpServer::UdpServer(const DeviceConfiguration& Configuration,
                     boost::asio::io_service &IOService,
                     MessageHandler& MessageHandler,
                     StateChangeListener<UdpServerState>& Listener) :
    StateBearer<UdpServerState>(Listener, UdpServerState::READY),
    m_IOService(IOService),
    m_MessageHandler(MessageHandler)
{
    Configuration.Get("Port", m_Port);
}

UdpServer::~UdpServer()
{
    Reset();
}

void UdpServer::Start()
{
    if(m_Socket) { return; }

    m_Socket = new udp::socket(m_IOService, udp::v4());
    boost::asio::socket_base::reuse_address ReuseAddressOption(true);
    m_Socket->set_option(ReuseAddressOption);

    try
    {
        m_Socket->bind(udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), m_Port));
        std::cout << "Accepting on port: " << m_Port << std::endl;
        SetState(UdpServerState::ACCEPTING);
        UdpServerSocketReader::AsyncRead(*m_Socket, *this);
    }
    catch (const boost::system::system_error& Error)
    {
        std::cerr << "UDP Server Start Error: " << Error.code().message() << std::endl;
        Reset();
        SetState(UdpServerState::ERROR);
    }
}


void UdpServer::Reset()
{
    if(!m_Socket) { return; }
    m_Socket->close();

    boost::unique_lock<boost::shared_mutex> Lock(m_SocketMutex);
    delete m_Socket;
    m_Socket = nullptr;
    SetState(UdpServerState::READY);
}


boost::shared_mutex& UdpServer::GetMutex()
{
    return m_SocketMutex;
}


void UdpServer::OnUdpReadSuccess(udp::endpoint Sender, const std::string& Message)
{
    (void)Sender;
    m_MessageHandler.OnUDPMessage(Message);
    UdpServerSocketReader::AsyncRead(*m_Socket, *this);
}


void UdpServer::OnUdpReadError(const std::string& Message)
{
    std::cerr << " UDP Read Error: " << Message << std::endl;
    SetState(UdpServerState::ERROR);
}

bool UdpServer::ReadyToStart() const
{
    return !m_Socket;
}
