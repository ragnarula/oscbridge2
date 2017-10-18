#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>
#include <QString>
#include "mainwindow.h"

namespace Ui {
class AddDeviceDialog;
}

class DeviceConfiguration;

class AddDeviceDialog : public QDialog
{
    Q_OBJECT
    friend class MainWindow;

public:
    explicit AddDeviceDialog(QWidget *parent = 0);
    void SetMainWindow(MainWindow* Window);
    void SetEditModeForDevice(const DeviceConfiguration& Configuration);

    ~AddDeviceDialog();

private slots:
    void NameChanged(const QString& NewName);

private:
    Ui::AddDeviceDialog *ui = nullptr;
    MainWindow* m_MainWindow = nullptr;
    bool m_EditMode = false;

    QString Name();
    QString Host();
    unsigned int Port();
};

#endif // ADDDEVICEDIALOG_H
