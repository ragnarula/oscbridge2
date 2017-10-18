#ifndef TCPDEVICERESOLVER_H
#define TCPDEVICERESOLVER_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>

class TcpClient;

class TcpClientResolver : public std::enable_shared_from_this<TcpClientResolver>
{
public:
    TcpClientResolver(std::string& Host,
                      unsigned int Port,
                      boost::asio::io_service& m_IOService,
                      TcpClient& Client);

    static void AsyncResolve(std::string& Host,
                             unsigned int Port,
                             boost::asio::io_service& m_IOService,
                             TcpClient& Client);

private:
    std::string m_Host;
    unsigned int m_Port;
    boost::asio::io_service& m_IOService;
    boost::asio::ip::tcp::resolver m_Resolver;
    boost::shared_lock<boost::shared_mutex> m_ClientSharedLock;
    TcpClient& m_Client;

    void Resolve();

    void HandleResolve(const boost::system::error_code& ErrorCode,
                       boost::asio::ip::tcp::resolver::iterator Endpoints);

    void TryAndConnect(boost::asio::ip::tcp::resolver::iterator Endpoints);
};

#endif // TCPDEVICERESOLVER_H
