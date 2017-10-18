#include "tcpclientresolver.h"
#include "tcpclient.h"
#include <iostream>
#include <string>
#include <boost/bind.hpp>


TcpClientResolver::TcpClientResolver(std::string& Host,
                                     unsigned int Port,
                                     boost::asio::io_service& IOService,
                                     TcpClient& Client) :
    m_Host(Host),
    m_Port(Port),
    m_IOService(IOService),
    m_Resolver(IOService),
    m_ClientSharedLock(Client.GetMutex()),
    m_Client(Client)
{

}

void TcpClientResolver::AsyncResolve(std::string& Host,
                                     unsigned int Port,
                                     boost::asio::io_service& IOService,
                                     TcpClient& Client)
{
    auto Resolver = std::make_shared<TcpClientResolver>(Host, Port, IOService, Client);
    Resolver->Resolve();
}


// TODO Remove Args
void TcpClientResolver::Resolve()
{
    boost::asio::ip::tcp::resolver::query query(m_Host, std::to_string(m_Port));
    m_Resolver.async_resolve(query,
                             boost::bind(&TcpClientResolver::HandleResolve,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::iterator)
                             );
}


void TcpClientResolver::HandleResolve(const boost::system::error_code& ErrorCode,
                   boost::asio::ip::tcp::resolver::iterator Endpoints)
{
    if(ErrorCode)
    {
        m_Client.OnTcpResolveError(ErrorCode.message());
    }
    else
    {
        TryAndConnect(Endpoints);
    }
}


void TcpClientResolver::TryAndConnect(boost::asio::ip::tcp::resolver::iterator Endpoints)
{
    boost::system::error_code HostError = boost::asio::error::host_not_found;
    boost::asio::ip::tcp::resolver::iterator end;

    boost::asio::ip::tcp::socket* Socket = new boost::asio::ip::tcp::socket(m_IOService);
    while(HostError && Endpoints != end)
    {
        Socket->close();
        Socket->connect(*Endpoints++, HostError);

        if(HostError)
        {
            Socket->close();
        }
    }

    if(!Socket->is_open())
    {
        m_Client.OnTcpResolveError(HostError.message());
        delete Socket;
    }
    else
    {
        m_Client.OnTcpResolveConnect(Socket);
    }
}

