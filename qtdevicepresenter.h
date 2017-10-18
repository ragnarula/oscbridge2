#ifndef QTDEVICEPRESENTER_H
#define QTDEVICEPRESENTER_H
#include "deviceinteractor.h"

#include <QObject>

class MainWindow;
class QString;

class QTDevicePresenter : public QObject, public DevicePresenter
{
    Q_OBJECT
public:
    QTDevicePresenter(MainWindow& MainWindow);
    virtual ~QTDevicePresenter() = default;
    void PresentDeviceState(std::map<std::string, std::string>& DeviceStates) override;

signals:
    void AddDeviceToView(const QString& DeviceName, const QString& DeviceState);
    void ClearDevices();
};

#endif // QTDEVICEPRESENTER_H
