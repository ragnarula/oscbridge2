#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include "statebearer.h"

enum class UdpServerState
{
    READY,
    ACCEPTING,
    ERROR
};

template <class T>
class StateChangeListener;

class MessageHandler;
class DeviceConfiguration;

class UdpServer : public StateBearer<UdpServerState>
{
public:
    UdpServer(unsigned int Port,
              boost::asio::io_service &IOService,
              MessageHandler& MessageHandler,
              StateChangeListener<UdpServerState>& Listener);

    UdpServer(const DeviceConfiguration& Configuration,
              boost::asio::io_service &IOService,
              MessageHandler& MessageHandler,
              StateChangeListener<UdpServerState>& Listener);

    ~UdpServer();

    void Start();
    void Reset();
    bool ReadyToStart() const;
    boost::shared_mutex& GetMutex();

    void OnUdpReadSuccess(boost::asio::ip::udp::endpoint Sender, const std::string& Message);
    void OnUdpReadError(const std::string& Message);

private:
    unsigned int m_Port;
    boost::asio::io_service& m_IOService;
    MessageHandler& m_MessageHandler;
    boost::asio::ip::udp::socket* m_Socket = nullptr;
    boost::shared_mutex m_SocketMutex;

};

#endif // UDPSERVER_H
