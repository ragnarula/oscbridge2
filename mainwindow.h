#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <string>
#include <QMap>

class DeviceConfiguration;
class QTSettingsDeviceConfigurationStore;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool ContainsDeviceName(const QString& Name);
    void InitFromStore(QTSettingsDeviceConfigurationStore& Store);

private slots:
    void StartServerButtonPushed();
    void StopServerButtonPushed();

    void AddButtonPushed();
    void RemoveButtonPushed();
    void EditButtonPushed();

    void StartButtonPushed();
    void StopButtonPushed();

public slots:
    void AddDeviceToView(const QString& DeviceName, const QString& DeviceState);
    void ClearDevices();
    void UpdateServerStatus(const QString& ServerStatus);

signals:
    void AddDialogSuccessfull(const char* Name, const char* Host, unsigned int Port);
    void AddDialogSuccessfull(const DeviceConfiguration& Configuration);
    void AddDevice(const char* Name);
    void RemoveDevice(const char* Name);
    void StartDevice(const char* Name);
    void StopDevice(const char* Name);

    void StartServer(const DeviceConfiguration& Configuration);
    void StopServer();

    void OnUpdateServerStatus(const QString& ServerStatus);

private:
    Ui::MainWindow *ui;
    QStandardItemModel m_Model;
    QMap<QString, DeviceConfiguration> m_DeviceConfigurations;
};

#endif // MAINWINDOW_H
