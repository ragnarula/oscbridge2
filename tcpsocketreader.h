#ifndef TCPDEVICEREADER_H
#define TCPDEVICEREADER_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>


class TcpClient;

class TcpSocketReader : public std::enable_shared_from_this<TcpSocketReader>
{
public:
    TcpSocketReader(boost::asio::ip::tcp::socket& Socket, TcpClient& Client);

    static void AsyncRead(boost::asio::ip::tcp::socket& Socket, TcpClient& Client);

private:
    std::vector<char> m_ReceiveBuffer;
    boost::asio::ip::tcp::socket& m_Socket;
    boost::shared_lock<boost::shared_mutex> m_ClientSharedLock;
    TcpClient& m_Client;

    void Read();
    void HandleReceive(const boost::system::error_code& ErrorCode, std::size_t BytesTransferred);
};

#endif // TCPDEVICEREADER_H
