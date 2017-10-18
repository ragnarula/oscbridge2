#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "devicestate.h"
#include "client.h"

template <class T>
class StateChangeListener;

class DeviceConfiguration;

class TcpClient : public Client
{
public:
    TcpClient(boost::asio::io_service& IOService,
              const std::string& Host,
              unsigned int Port,
              StateChangeListener<ClientState> &Listener);
    TcpClient(boost::asio::io_service& IOService,
              const DeviceConfiguration& Configuration,
              StateChangeListener<ClientState> &Listener);
    virtual ~TcpClient();

    // Device
    void Start() override;
    void Reset() override;
    bool ReadyToStart() const override;
    void Write(const std::string& Message) override;

    // Client
    boost::shared_mutex& GetMutex();

    // Resolver
    void OnTcpResolveConnect(boost::asio::ip::tcp::socket* Socket);
    void OnTcpResolveError(const std::string& Message);

    // Reader
    void OnTcpReadSuccess(const std::string& Message);
    void OnTcpReadError(const std::string& Message);

    // Writer
    void OnTcpWriteSuccess(std::size_t BytesTransferred);
    void OnTcpWriteError(const std::string& Message);

private:
    boost::shared_mutex m_DestructorMutex;
    boost::asio::ip::tcp::socket* m_Socket = nullptr;
    boost::asio::io_service& m_IOService;
    std::string m_Host;
    unsigned int m_Port;
};

#endif // TCPCLIENT_H
