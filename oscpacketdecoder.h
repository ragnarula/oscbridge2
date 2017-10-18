#ifndef OSCPACKETDECODER_H
#define OSCPACKETDECODER_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>

class MessageHandler;

class OSCPacketDecoder : public std::enable_shared_from_this<OSCPacketDecoder>
{
public:
    OSCPacketDecoder(const std::string& Message,
                     boost::asio::io_service& IOService,
                     MessageHandler& MessageHandler);

    static void AsyncDecode(const std::string& Message,
                            boost::asio::io_service& IOService,
                            MessageHandler& MessageHandler);

private:
    std::string m_Message;
    boost::asio::io_service& m_IOService;
    MessageHandler& m_MessageHandler;
    boost::shared_lock<boost::shared_mutex> m_MessageHandlerSharedLock;

    void BeginDecode();

    void DoDecode();
};

#endif // OSCPACKETDECODER_H
