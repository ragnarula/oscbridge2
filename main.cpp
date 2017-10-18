#include "mainwindow.h"
#include <QApplication>
#include "ioservicepool.h"
#include <boost/asio.hpp>
#include "tcpclient.h"
#include "messagehandler.h"
#include "deviceinteractor.h"
#include "qtdeviceinteractor.h"
#include "qtdevicepresenter.h"
#include "qtsettingsdeviceconfigurationstore.h"
#include "qtserverpresenter.h"
#include "serverinteractor.h"
#include "qtserverinteractor.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QApplication::setApplicationName("OSC Bridge 2");
    QApplication::setOrganizationName("Raghav Narula Software Co.");

    MainWindow Window;
    boost::asio::io_service IOService;
    IOServicePool IOPool(&IOService, 2);

    IOPool.Start();
    QTSettingsDeviceConfigurationStore Store;



    QTDevicePresenter Presenter(Window);
    DeviceInteractor Interactor(Presenter, Store);
    QTDeviceInteractor QTInteractor(Interactor, Window, IOService);
//    Interactor.InitFromStore(IOService);

    MessageHandler MessageHandler(IOService, Interactor);

    QTServerPresenter ServerPresenter(Window);
    ServerInteractor SInteractor(ServerPresenter, Store);
    QTServerInteractor QTSInteractor(SInteractor, Window, IOService, MessageHandler);
//    SInteractor.InitFromStore(IOService, MessageHandler);
    Window.InitFromStore(Store);
    Window.show();
    return a.exec();
}
