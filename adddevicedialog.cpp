#include "adddevicedialog.h"
#include "ui_adddevicedialog.h"
#include "deviceconfiguration.h"
#include <QAbstractButton>

AddDeviceDialog::AddDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDeviceDialog)
{
    ui->setupUi(this);
    connect(ui->NameEdit,
            SIGNAL(textChanged(const QString&)),
            this,
            SLOT(NameChanged(const QString&)));
    ui->buttonBox->buttons().at(0)->setEnabled(false);
}

AddDeviceDialog::~AddDeviceDialog()
{
    delete ui;
}


QString AddDeviceDialog::Name()
{
    return ui->NameEdit->text();
}


QString AddDeviceDialog::Host()
{
    return ui->HostEdit->text();
}


unsigned int AddDeviceDialog::Port()
{
    return ui->PortEdit->value();
}


void AddDeviceDialog::SetMainWindow(MainWindow *Window)
{
    m_MainWindow = Window;
}


void AddDeviceDialog::SetEditModeForDevice(const DeviceConfiguration& Configuration)
{
    m_EditMode = true;
    ui->TitleLabel->setText("Edit TCP Device");
    std::string Name;
    std::string Host;
    int Port;

    Configuration.Get("Name", Name);
    Configuration.Get("Host", Host);
    Configuration.Get("Port", Port);

    ui->NameTakenLabel->setVisible(false);
    ui->NameEdit->setText(QString::fromStdString(Name));
    ui->NameEdit->setEnabled(false);

    ui->HostEdit->setText(QString::fromStdString(Host));
    ui->PortEdit->setValue(Port);

    ui->buttonBox->buttons().at(0)->setEnabled(true);
}

void AddDeviceDialog::NameChanged(const QString& NewName)
{
    if (!m_MainWindow) { return; }
    if (m_MainWindow->ContainsDeviceName(NewName))
    {
        ui->NameTakenLabel->setText("Invalid");
        ui->buttonBox->buttons().at(0)->setEnabled(false);
    }
    else
    {
        ui->NameTakenLabel->setText("Valid");
        ui->buttonBox->buttons().at(0)->setEnabled(true);
    }
}
