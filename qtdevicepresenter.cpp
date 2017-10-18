#include "qtdevicepresenter.h"
#include <iostream>
#include "mainwindow.h"
#include <QString>

QTDevicePresenter::QTDevicePresenter(MainWindow &Window)
{
    QObject::connect(this, SIGNAL(ClearDevices()),
                     &Window, SLOT(ClearDevices()), Qt::QueuedConnection);

    QObject::connect(this, SIGNAL(AddDeviceToView(const QString&, const QString&)),
                     &Window, SLOT(AddDeviceToView(const QString&, const QString&)), Qt::QueuedConnection);
}


void QTDevicePresenter::PresentDeviceState(std::map<std::string, std::string>& DeviceStates)
{
    emit ClearDevices();
    for(const auto& Iterator : DeviceStates){
        emit AddDeviceToView(QString::fromStdString(Iterator.first), QString::fromStdString(Iterator.second));
    }
}
