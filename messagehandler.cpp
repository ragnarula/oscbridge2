#include "messagehandler.h"
#include "oscpacketdecoder.h"
#include "oscmessagedecoder.h"
#include "deviceinteractor.h"
#include <iostream>

MessageHandler::MessageHandler(boost::asio::io_service& IOService, DeviceInteractor& Interactor) :
    m_IOService(IOService),
    m_DeviceInteractor(Interactor)
{

}

void MessageHandler::OnUDPMessage(const std::string &Message)
{
    OSCPacketDecoder::AsyncDecode(Message, m_IOService, *this);
}

boost::shared_mutex& MessageHandler::GetMutex()
{
    return m_Mutex;
}

void MessageHandler::OnOSCMessageParseSuccess(const oscpkt::Message& Message)
{
    OSCMessageDecoder::AsyncDecode(Message, m_IOService, *this);
}


void MessageHandler::OnOSCMessageParseError(const oscpkt::ErrorCode& ErrorCode)
{
    std::cerr << "OSC Error: " << ErrorCode << std::endl;
}

void MessageHandler::OnDeviceMessage(const std::string& DeviceName, const std::string& Message)
{
    m_DeviceInteractor.WriteToDevice(DeviceName, Message);
}
