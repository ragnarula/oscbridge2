#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QObject>
#include <boost/asio.hpp>

class MainWindow;
class DeviceInteractor;
class DeviceConfiguration;


class QTDeviceInteractor : public QObject
{
    Q_OBJECT
public:
    QTDeviceInteractor(DeviceInteractor& Interactor, MainWindow& Window, boost::asio::io_service& IOService);

private:
    DeviceInteractor& m_DeviceInteractor;
    boost::asio::io_service& m_IOService;

public slots:
    void CreateNewTcpDevice(const char* Name,
                            const char* Host,
                            unsigned int Port);

    void CreateNewDevice(const DeviceConfiguration& Configuration);

    void StartDevice(const char* Name);
    void StopDevice(const char* Name);
    void RemoveDevice(const char* Name);
};

#endif // VIEWMODEL_H
