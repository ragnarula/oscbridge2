#include "udpserversocketreader.h"
#include "udpserver.h"
#include <boost/bind.hpp>
#include <iostream>

UdpServerSocketReader::UdpServerSocketReader(boost::asio::ip::udp::socket& Socket, UdpServer& Server) :
    m_Socket(Socket),
    m_ServerSharedLock(Server.GetMutex()),
    m_Server(Server)
{
    boost::asio::socket_base::receive_buffer_size BufferSizeOption;
    m_Socket.get_option(BufferSizeOption);

    int Size = BufferSizeOption.value();
    m_ReceiveBuffer = std::vector<char>(Size);

    std::cout << "UDP Socket buffer size: " << Size << std::endl;
}

void UdpServerSocketReader::AsyncRead(boost::asio::ip::udp::socket &Socket, UdpServer &Server)
{
    auto Reader = std::make_shared<UdpServerSocketReader>(Socket, Server);
    Reader->Read();
}

void UdpServerSocketReader::Read()
{
    m_Socket.async_receive_from(boost::asio::buffer(m_ReceiveBuffer),
                                m_SenderEndpoint,
                                boost::bind(&UdpServerSocketReader::HandleReceive,
                                            shared_from_this(),
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred())
                                );
}

void UdpServerSocketReader::HandleReceive(const boost::system::error_code &ErrorCode, std::size_t BytesTransferred)
{
    if(ErrorCode)
    {
        m_Server.OnUdpReadError(ErrorCode.message());
    }
    else
    {
        m_Server.OnUdpReadSuccess(m_SenderEndpoint, std::string(&m_ReceiveBuffer[0], BytesTransferred));
    }
}
