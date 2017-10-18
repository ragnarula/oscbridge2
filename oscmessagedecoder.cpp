#include "oscmessagedecoder.h"
#include "messagehandler.h"
#include <boost/bind.hpp>
#include <sstream>
#include <iostream>


OSCMessageDecoder::OSCMessageDecoder(const oscpkt::Message& Message,
                                     boost::asio::io_service& IOService,
                                     MessageHandler& MessageHandler) :
    m_Message(Message),
    m_MessageHandlerSharedLock(MessageHandler.GetMutex()),
    m_IOService(IOService),
    m_MessageHandler(MessageHandler)
{

}

void OSCMessageDecoder::AsyncDecode(const oscpkt::Message &Message,
                                    boost::asio::io_service &IOService,
                                    MessageHandler &MessageHandler)
{
    auto Decoder = std::make_shared<OSCMessageDecoder>(Message, IOService, MessageHandler);
    Decoder->BeginDecode();
}

void OSCMessageDecoder::BeginDecode()
{
    m_IOService.post(boost::bind(&OSCMessageDecoder::DoDecode, shared_from_this()));
}

void OSCMessageDecoder::DoDecode()
{
    const std::string AddressPattern = m_Message.addressPattern();
    std::istringstream AddressPatternStream(AddressPattern);
    std::string Part;
    std::getline(AddressPatternStream, Part, '/'); // skip first

    if (std::getline(AddressPatternStream, Part, '/') && Part == "device")
    {
        DecodeDeviceMessage(AddressPatternStream);
    }
    else
    {
        std::cout << "Message Type Error" << std::endl;
        // error - non device message
    }
}

void OSCMessageDecoder::DecodeDeviceMessage(std::istringstream& AddressPatternStream)
{
    // Require Target and Function
    std::string Target;
    std::string Function;


    if (!std::getline(AddressPatternStream, Target, '/'))
    {
        std::cout << "Message Target Error" << std::endl;
        // error - no Target
        return;
    }

    if (!std::getline(AddressPatternStream, Function, '/'))
    {
        std::cout << "Message Function Error" << std::endl;
        // error - No Type
        return;
    }

    if (Function != "message")
    {
        std::cout << "Message Unknown Function Error" << std::endl;
        // error - bad type
        return;
    }

    std::string Message; // extract message
    if (!m_Message.arg().popStr(Message).isOkNoMoreArgs())
    {
        std::cout << "Message Content Error" << std::endl;
        // error - Can't extract string message
        return;
    }

    m_MessageHandler.OnDeviceMessage(Target, Message);
}
