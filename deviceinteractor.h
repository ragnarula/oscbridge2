#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <boost/asio.hpp>
#include <map>
#include <string>
#include "statechangelistener.h"
#include "clientstate.h"

class TcpClient;
class DeviceConfiguration;
class DeviceConfigurationStore;

class DevicePresenter
{
public:
    virtual void PresentDeviceState(std::map<std::string, std::string>& DeviceStates) = 0;
};


class DeviceInteractor : public StateChangeListener<ClientState>
{
public:
    DeviceInteractor(DevicePresenter& Presenter, DeviceConfigurationStore& Store);
    virtual ~DeviceInteractor();
    void InitFromStore(boost::asio::io_service& IOService);
    void CreateNewTcpDevice(const std::string& Name,
                            const std::string& Host,
                            unsigned int Port,
                            boost::asio::io_service& IOService);

    void CreateNewDevice(const DeviceConfiguration& Configuration,
                         boost::asio::io_service& IOService);
    void StartDevice(const std::string& Name);
    void StopDevice(const std::string& Name);
    void RemoveDevice(const std::string& Name);
    void WriteToDevice(const std::string& Name, const std::string& Message);

    // Device State Change Listener
    void OnStateChange() override;

private:
    DevicePresenter& m_DevicePresenter;
    DeviceConfigurationStore& m_ConfigurationStore;
    std::map<std::string, TcpClient*> m_DeviceMap;

    void PresentDevices();
};

#endif // DEVICEMANAGER_H
