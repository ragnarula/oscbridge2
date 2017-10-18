#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "oscpkt.hh"

class DeviceInteractor;

class MessageHandler
{
public:
    MessageHandler(boost::asio::io_service& IOService, DeviceInteractor& Interactor);

    void OnUDPMessage(const std::string& Message);
    boost::shared_mutex& GetMutex();

    // OSC Packet Decoder
    void OnOSCMessageParseSuccess(const oscpkt::Message& Message);
    void OnOSCMessageParseError(const oscpkt::ErrorCode& ErrorCode);

    // OSC Message Decoder
    void OnDeviceMessage(const std::string& DeviceName, const std::string& Message);

private:
    boost::shared_mutex m_Mutex;
    boost::asio::io_service& m_IOService;
    DeviceInteractor& m_DeviceInteractor;
};

#endif // MESSAGEPARSER_H
