#include "tcpsocketwriter.h"
#include "tcpclient.h"
#include <boost/bind.hpp>
#include <iostream>

TcpSocketWriter::TcpSocketWriter(const std::string& Message, boost::asio::ip::tcp::socket& Socket, TcpClient& Client) :
    m_Message(Message),
    m_Socket(Socket),
    m_ClientSharedLock(Client.GetMutex()),
    m_Client(Client)
{

}


void TcpSocketWriter::AsyncWrite(const std::string& Message, boost::asio::ip::tcp::socket& Socket, TcpClient& Client)
{
    auto Writer = std::make_shared<TcpSocketWriter>(Message, Socket, Client);
    Writer->Write();
}


void TcpSocketWriter::Write()
{
    boost::asio::async_write(m_Socket,
                             boost::asio::buffer(m_Message),
                             boost::bind(&TcpSocketWriter::HandleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::iterator)
                             );
}


void TcpSocketWriter::HandleWrite(const boost::system::error_code &ErrorCode, std::size_t BytesTransferred)
{
    if(ErrorCode)
    {
        m_Client.OnTcpWriteError(ErrorCode.message());
    }
    else
    {
        m_Client.OnTcpWriteSuccess(BytesTransferred);
    }
}


