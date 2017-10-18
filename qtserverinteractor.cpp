#include "qtserverinteractor.h"
#include "serverinteractor.h"
#include "mainwindow.h"
#include "messagehandler.h"

QTServerInteractor::QTServerInteractor(ServerInteractor& Interactor,
                                       MainWindow& Window,
                                       boost::asio::io_service& IOService,
                                       MessageHandler& MessageHandler) :
    m_ServerInteractor(Interactor),
    m_IOService(IOService),
    m_MessageHandler(MessageHandler)
{
    QObject::connect(&Window, SIGNAL(StartServer(const DeviceConfiguration&)),
                     this, SLOT(StartServer(const DeviceConfiguration&)));

    QObject::connect(&Window, SIGNAL(StopServer()),
                     this, SLOT(StopServer()));
}


void QTServerInteractor::StartServer(const DeviceConfiguration& Configuration)
{
    m_ServerInteractor.StartServer(Configuration, m_IOService, m_MessageHandler);
}

void QTServerInteractor::StopServer()
{
    m_ServerInteractor.StopServer();
}
