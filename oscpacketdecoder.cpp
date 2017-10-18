#include "oscpacketdecoder.h"
#include "messagehandler.h"
#include "oscpkt.hh"
#include <boost/bind.hpp>


OSCPacketDecoder::OSCPacketDecoder(const std::string& Message,
                                   boost::asio::io_service& IOService,
                                   MessageHandler& MessageHandler) :
    m_Message(Message),
    m_IOService(IOService),
    m_MessageHandler(MessageHandler),
    m_MessageHandlerSharedLock(m_MessageHandler.GetMutex())
{

}

void OSCPacketDecoder::AsyncDecode(const std::string &Message,
                                   boost::asio::io_service &IOService,
                                   MessageHandler &MessageHandler)
{
    auto Decoder = std::make_shared<OSCPacketDecoder>(Message,
                                                      IOService,
                                                      MessageHandler);
    Decoder->BeginDecode();
}

void OSCPacketDecoder::BeginDecode()
{
    m_IOService.post(boost::bind(&OSCPacketDecoder::DoDecode, shared_from_this()));
}


void OSCPacketDecoder::DoDecode()
{
    oscpkt::PacketReader OSCPacketReader;
    OSCPacketReader.init(m_Message.c_str(), m_Message.size());
    oscpkt::Message* OSCMessage;

    if(!OSCPacketReader.isOk())
    {
        m_MessageHandler.OnOSCMessageParseError(OSCPacketReader.getErr());
    }

    while(OSCPacketReader.isOk() && (OSCMessage = OSCPacketReader.popMessage()) != 0)
    {
        m_MessageHandler.OnOSCMessageParseSuccess(*OSCMessage);
    }
}

