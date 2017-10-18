#include "tcpclient.h"
#include "tcpsocketreader.h"
#include "tcpsocketwriter.h"
#include "tcpclientresolver.h"
#include "deviceconfiguration.h"
#include <iostream>

TcpClient::TcpClient(boost::asio::io_service& IOService,
                     const std::string& Host,
                     unsigned int Port,
                     StateChangeListener<ClientState> &Listener) :
    Client(Listener),
    m_IOService(IOService),
    m_Host(Host),
    m_Port(Port)
{
    SetState(ClientState::READY);
}

TcpClient::TcpClient(boost::asio::io_service& IOService,
                     const DeviceConfiguration& Configuration,
                     StateChangeListener<ClientState> &Listener) :
    Client(Listener),
    m_IOService(IOService)
{
    Configuration.Get("Host", m_Host);
    Configuration.Get("Port", m_Port);
    SetState(ClientState::READY);
}

TcpClient::~TcpClient()
{
    Reset();
}


// Client
void TcpClient::Start()
{    
    if(GetState() != ClientState::READY) { return; }
    SetState(ClientState::CONNECTING);
    TcpClientResolver::AsyncResolve(m_Host, m_Port, m_IOService, *this);
}


void TcpClient::Reset()
{
    if (m_Socket)
    {
        boost::system::error_code ec;
        m_Socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        m_Socket->close();
        boost::unique_lock<boost::shared_mutex> Lock(m_DestructorMutex);
        delete m_Socket;
        m_Socket = nullptr;
    }
    SetState(ClientState::READY);
}


bool TcpClient::ReadyToStart() const
{
    return !m_Socket;
}


void TcpClient::Write(const std::string &Message)
{
    if(!m_Socket) { return; }
    TcpSocketWriter::AsyncWrite(Message, *m_Socket, *this);
}


boost::shared_mutex& TcpClient::GetMutex()
{
    return m_DestructorMutex;
}


// Resolver
void TcpClient::OnTcpResolveConnect(boost::asio::ip::tcp::socket* Socket)
{
    m_Socket = Socket;
    SetState(ClientState::CONNECTED);
    TcpSocketReader::AsyncRead(*m_Socket, *this);
}

void TcpClient::OnTcpResolveError(const std::string& Message)
{
    std::cerr << "[" << m_Host << "] Resolve Error: " << Message << std::endl;
    SetState(ClientState::ERROR);
}


// Reader
void TcpClient::OnTcpReadSuccess(const std::string& Message)
{
    std::cout << "[" << m_Host << "] Received: " << Message << std::endl;
    TcpSocketReader::AsyncRead(*m_Socket, *this);
}


void TcpClient::OnTcpReadError(const std::string& Message)
{
    std::cerr << "[" << m_Host << "] Read Error: " << Message << std::endl;
    SetState(ClientState::ERROR);
}


// Writer
void TcpClient::OnTcpWriteSuccess(std::size_t BytesTransferred)
{
    std::cout << "[" << m_Host << "] Sent: " << BytesTransferred <<  " Bytes" << std::endl;
}


void TcpClient::OnTcpWriteError(const std::string& Message)
{
    std::cerr << "[" << m_Host << "] Write Error:" << Message << std::endl;
    SetState(ClientState::ERROR);
}
