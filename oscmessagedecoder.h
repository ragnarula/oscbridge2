#ifndef OSCMESSAGEDECODER_H
#define OSCMESSAGEDECODER_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "oscpkt.hh"

class MessageHandler;

class OSCMessageDecoder : public std::enable_shared_from_this<OSCMessageDecoder>
{
public:
    OSCMessageDecoder(const oscpkt::Message& Message,
                      boost::asio::io_service& IOService,
                      MessageHandler& MessageHandler);

    static void AsyncDecode(const oscpkt::Message& Message,
                            boost::asio::io_service& IOService,
                            MessageHandler& MessageHandler);

private:
    oscpkt::Message m_Message;
    boost::shared_lock<boost::shared_mutex> m_MessageHandlerSharedLock;
    boost::asio::io_service& m_IOService;
    MessageHandler &m_MessageHandler;
    void BeginDecode();
    void DoDecode();

    void DecodeDeviceMessage(std::istringstream& AddressPatternStream);
};

#endif // OSCMESSAGEDECODER_H
