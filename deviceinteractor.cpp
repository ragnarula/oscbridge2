#include "deviceinteractor.h"
#include "tcpclient.h"
#include "deviceconfiguration.h"
#include "deviceconfigurationstore.h"

DeviceInteractor::DeviceInteractor(DevicePresenter& Presenter, DeviceConfigurationStore& Store) :
    m_DevicePresenter(Presenter),
    m_ConfigurationStore(Store)
{
    Store.GetAllDeviceConfigurations();
}

DeviceInteractor::~DeviceInteractor()
{
    for(auto const& Iterator : m_DeviceMap)
    {
        delete Iterator.second;
    }
}

void DeviceInteractor::InitFromStore(boost::asio::io_service& IOService)
{
    auto StoredConfigurations = m_ConfigurationStore.GetAllDeviceConfigurations();
    for(auto& Configuration : StoredConfigurations)
    {
        CreateNewDevice(Configuration, IOService);
    }
}

void DeviceInteractor::CreateNewTcpDevice(const std::string& Name,
                        const std::string& Host,
                        unsigned int Port,
                        boost::asio::io_service& IOService)
{
    if (m_DeviceMap.find(Name) != m_DeviceMap.end()) { return; } // key exists
    TcpClient* TcpDevice = new TcpClient(IOService, Host, Port, *this);
    m_DeviceMap[Name] = TcpDevice;
    PresentDevices();
}


void DeviceInteractor::CreateNewDevice(const DeviceConfiguration& Configuration,
                                       boost::asio::io_service& IOService)
{
    std::string DeviceType;
    std::string Name;
    if(Configuration.Get("Type", DeviceType) && DeviceType == "TCP_CLIENT" && // TODO Remove constant
       Configuration.Get("Name", Name))
    {
        if (m_DeviceMap.find(Name) != m_DeviceMap.end()) { return; } // key exists
        TcpClient* TcpDevice = new TcpClient(IOService, Configuration, *this);
        m_DeviceMap[Name] = TcpDevice;
        m_ConfigurationStore.StoreDeviceConfiguration(Name, Configuration);
        PresentDevices();
    }
}

void DeviceInteractor::StartDevice(const std::string& Name)
{
    if (m_DeviceMap.find(Name) == m_DeviceMap.end()) return; // Key Doesn't Exist
    if(m_DeviceMap[Name]->ReadyToStart()) m_DeviceMap[Name]->Start();
}

void DeviceInteractor::StopDevice(const std::string& Name)
{
    if (m_DeviceMap.find(Name) == m_DeviceMap.end()) return; // Key Doesn't Exist
    m_DeviceMap[Name]->Reset();
}

void DeviceInteractor::RemoveDevice(const std::string& Name)
{
    if (m_DeviceMap.find(Name) == m_DeviceMap.end()) return; // Key Doesn't Exist
    delete m_DeviceMap[Name];
    m_DeviceMap.erase(Name);
    m_ConfigurationStore.RemoveDeviceConfiguration(Name);
    PresentDevices();
}

void DeviceInteractor::WriteToDevice(const std::string& Name, const std::string& Message)
{
    if (m_DeviceMap.find(Name) == m_DeviceMap.end()) return; // Key Doesn't Exist
    m_DeviceMap[Name]->Write(Message);
}


// Device State Change Listener
void DeviceInteractor::OnStateChange()
{
    PresentDevices();
}


void DeviceInteractor::PresentDevices()
{
    std::map<std::string, std::string> DeviceStates;
    for (auto& Iterator : m_DeviceMap)
    {
        auto CurrentDeviceState = Iterator.second->GetState();
        switch(CurrentDeviceState)
        {
        case ClientState::READY:
            DeviceStates[Iterator.first] = "Ready";
            break;
        case ClientState::CONNECTING:
            DeviceStates[Iterator.first] = "Connecting";
            break;
        case ClientState::CONNECTED:
            DeviceStates[Iterator.first] = "Connected";
            break;
        case ClientState::ERROR:
            DeviceStates[Iterator.first] = "Error";
            break;
         default:
            DeviceStates[Iterator.first] = "Unknown";
        }
    }
    m_DevicePresenter.PresentDeviceState(DeviceStates);
}
