#ifndef TCPDEVICEWRITER_H
#define TCPDEVICEWRITER_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

class TcpClient;

class TcpSocketWriter : public std::enable_shared_from_this<TcpSocketWriter>
{
public:
    TcpSocketWriter(const std::string& Message, boost::asio::ip::tcp::socket& Socket, TcpClient& Client);

    static void AsyncWrite(const std::string& Message, boost::asio::ip::tcp::socket& Socket, TcpClient& Client);

private:
    std::string m_Message;
    boost::asio::ip::tcp::socket& m_Socket;
    boost::shared_lock<boost::shared_mutex> m_ClientSharedLock;
    TcpClient& m_Client;

    void Write();
    void HandleWrite(const boost::system::error_code& ErrorCode, std::size_t BytesTransferred);
};

#endif // TCPDEVICEWRITER_H
