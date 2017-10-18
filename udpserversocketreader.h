#ifndef UDPSERVERSOCKETREADER_H
#define UDPSERVERSOCKETREADER_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>

class UdpServer;

class UdpServerSocketReader : public std::enable_shared_from_this<UdpServerSocketReader>
{
public:
    UdpServerSocketReader(boost::asio::ip::udp::socket& Socket, UdpServer& Server);

    static void AsyncRead(boost::asio::ip::udp::socket& Socket, UdpServer& Server);

private:
    std::vector<char> m_ReceiveBuffer;
    boost::asio::ip::udp::endpoint m_SenderEndpoint;
    boost::asio::ip::udp::socket& m_Socket;
    boost::shared_lock<boost::shared_mutex> m_ServerSharedLock;
    UdpServer& m_Server;

    void Read();
    void HandleReceive(const boost::system::error_code& ErrorCode, std::size_t BytesTransferred);
};

#endif // UDPSERVERSOCKETREADER_H
