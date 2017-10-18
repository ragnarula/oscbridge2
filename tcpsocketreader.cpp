#include "tcpsocketreader.h"
#include "tcpclient.h"
#include <iostream>
#include <boost/bind.hpp>

TcpSocketReader::TcpSocketReader(boost::asio::ip::tcp::socket& Socket, TcpClient& Client) :
    m_Socket(Socket),
    m_ClientSharedLock(Client.GetMutex()),
    m_Client(Client)
{
    boost::asio::socket_base::receive_buffer_size BufferSizeOption;
    m_Socket.get_option(BufferSizeOption);

    int Size = BufferSizeOption.value();
    m_ReceiveBuffer = std::vector<char>(Size);
}

void TcpSocketReader::AsyncRead(boost::asio::ip::tcp::socket& Socket, TcpClient& Client)
{
    auto Reader = std::make_shared<TcpSocketReader>(Socket, Client);
    Reader->Read();
}

void TcpSocketReader::Read()
{
    m_Socket.async_receive(boost::asio::buffer(m_ReceiveBuffer),
                         boost::bind(&TcpSocketReader::HandleReceive, shared_from_this(),
                                     boost::asio::placeholders::error(),
                                     boost::asio::placeholders::bytes_transferred())
                         );
}

void TcpSocketReader::HandleReceive(const boost::system::error_code &ErrorCode, std::size_t BytesTransferred)
{
    if(ErrorCode)
    {
        m_Client.OnTcpReadError(ErrorCode.message());
        // TODO Log error code
    }
    else
    {
        m_Client.OnTcpReadSuccess(std::string(&m_ReceiveBuffer[0], BytesTransferred));
    }
}
