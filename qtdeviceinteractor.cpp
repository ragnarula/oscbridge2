#include "qtdeviceinteractor.h"
#include "mainwindow.h"
#include "deviceinteractor.h"
#include "deviceconfiguration.h"

QTDeviceInteractor::QTDeviceInteractor(DeviceInteractor& Interactor, MainWindow& Window, boost::asio::io_service& IOService) :
    m_DeviceInteractor(Interactor),
    m_IOService(IOService)
{
    QObject::connect(&Window, SIGNAL(AddDialogSuccessfull(const char*, const char*, unsigned int)),
                     this, SLOT(CreateNewTcpDevice(const char*, const char*, unsigned int)));

    QObject::connect(&Window, SIGNAL(AddDialogSuccessfull(const DeviceConfiguration&)),
                     this, SLOT(CreateNewDevice(const DeviceConfiguration&)));

    QObject::connect(&Window, SIGNAL(RemoveDevice(const char*)),
                     this, SLOT(RemoveDevice(const char*)));

    QObject::connect(&Window, SIGNAL(StartDevice(const char*)),
                     this, SLOT(StartDevice(const char*)));

    QObject::connect(&Window, SIGNAL(StopDevice(const char*)),
                     this, SLOT(StopDevice(const char*)));
}

// slots
void QTDeviceInteractor::CreateNewTcpDevice(const char* Name,
                        const char* Host,
                        unsigned int Port)
{
    m_DeviceInteractor.CreateNewTcpDevice(std::string(Name), std::string(Host), Port, m_IOService);
}

void QTDeviceInteractor::StartDevice(const char* Name)
{
    m_DeviceInteractor.StartDevice(std::string(Name));
}


void QTDeviceInteractor::StopDevice(const char* Name)
{
    m_DeviceInteractor.StopDevice(std::string(Name));
}


void QTDeviceInteractor::RemoveDevice(const char* Name)
{
    m_DeviceInteractor.RemoveDevice(std::string(Name));
}

void QTDeviceInteractor::CreateNewDevice(const DeviceConfiguration& Configuration)
{
    m_DeviceInteractor.CreateNewDevice(Configuration, m_IOService);
}
