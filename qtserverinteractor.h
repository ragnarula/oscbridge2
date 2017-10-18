#ifndef QTSERVERINTERACTOR_H
#define QTSERVERINTERACTOR_H

#include <QObject>
#include <boost/asio.hpp>

class MainWindow;
class ServerInteractor;
class DeviceConfiguration;
class MessageHandler;

class QTServerInteractor : public QObject
{
    Q_OBJECT
public:
    QTServerInteractor(ServerInteractor& Interactor,
                       MainWindow& Window,
                       boost::asio::io_service& IOService,
                       MessageHandler& MessageHandler);

private:
    ServerInteractor& m_ServerInteractor;
    boost::asio::io_service& m_IOService;
    MessageHandler& m_MessageHandler;

public slots:
    void StartServer(const DeviceConfiguration& Configuration);
    void StopServer();
};

#endif // QTSERVERINTERACTOR_H
