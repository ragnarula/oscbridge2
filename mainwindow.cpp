#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adddevicedialog.h"
#include "ui_adddevicedialog.h"
#include "deviceconfiguration.h"
#include "qtsettingsdeviceconfigurationstore.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Model(1, 2)
{
    ui->setupUi(this);
    QObject::connect(ui->AddDeviceButton, SIGNAL(clicked()),
            this, SLOT(AddButtonPushed()));
    QObject::connect(ui->RemoveDeviceButton, SIGNAL(clicked()),
            this, SLOT(RemoveButtonPushed()));
    QObject::connect(ui->EditDeviceButton, SIGNAL(clicked()),
            this, SLOT(EditButtonPushed()));
    QObject::connect(ui->StartDeviceButton, SIGNAL(clicked()),
            this, SLOT(StartButtonPushed()));
    QObject::connect(ui->StopDeviceButton, SIGNAL(clicked()),
            this, SLOT(StopButtonPushed()));

    QObject::connect(ui->ServerStartButton, SIGNAL(clicked()),
                     this, SLOT(StartServerButtonPushed()));

    QObject::connect(ui->ServerStopButton, SIGNAL(clicked()),
                     this, SLOT(StopServerButtonPushed()));

    QObject::connect(this, SIGNAL(OnUpdateServerStatus(QString)),
                     ui->ServerStatusLabel, SLOT(setText(QString)));

    QStringList Labels;
    Labels << "Name" << "Status";
    m_Model.setHorizontalHeaderLabels(Labels);

    ui->deviceView->verticalHeader()->setVisible(false);
    ui->deviceView->horizontalHeader()->setStretchLastSection(true);

    ui->deviceView->setModel(&m_Model);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::StartServerButtonPushed()
{
    auto Port = ui->PortSipinBox->value();
    DeviceConfiguration Configuration;
    Configuration.Set("Type", "UDP_SERVER");
    Configuration.Set("Port", Port);
    emit StartServer(Configuration);
}

void MainWindow::StopServerButtonPushed()
{
    emit StopServer();
}

bool MainWindow::ContainsDeviceName(const QString& Name)
{
    QList<QStandardItem *> MatchedItems = m_Model.findItems(Name, Qt::MatchExactly, 0);
    return MatchedItems.size() > 0;
}

// private slots
void MainWindow::AddButtonPushed()
{
    AddDeviceDialog Dialog;
    Dialog.SetMainWindow(this);

    if(Dialog.exec() == AddDeviceDialog::Accepted)
    {
        DeviceConfiguration Configuration;
        Configuration.Set("Type", "TCP_CLIENT");
        Configuration.Set("Name", Dialog.Name().toStdString());
        Configuration.Set("Host", Dialog.Host().toStdString());
        Configuration.Set("Port", Dialog.Port());

        m_DeviceConfigurations[Dialog.Name()] = Configuration;

        emit AddDialogSuccessfull(Configuration);
        //emit AddDialogSuccessfull(Dialog.Name().c_str(), Dialog.Host().c_str(), Dialog.Port());
    }
}

void MainWindow::EditButtonPushed()
{
    QModelIndexList indexes = ui->deviceView->selectionModel()->selection().indexes();

    for(int i = 0; i < indexes.count(); ++i)
    {
        QModelIndex index = indexes.at(i);
        QStandardItem *item = m_Model.item(index.row());

        DeviceConfiguration OldConfiguration = m_DeviceConfigurations[item->text()];

        AddDeviceDialog Dialog;
        Dialog.SetMainWindow(this);
        Dialog.SetEditModeForDevice(OldConfiguration);

        if(Dialog.exec() == AddDeviceDialog::Accepted)
        {
            emit RemoveDevice(Dialog.Name().toLocal8Bit().data());
            DeviceConfiguration Configuration;
            Configuration.Set("Type", "TCP_CLIENT");
            Configuration.Set("Name", Dialog.Name().toStdString());
            Configuration.Set("Host", Dialog.Host().toStdString());
            Configuration.Set("Port", Dialog.Port());
            m_DeviceConfigurations[Dialog.Name()] = Configuration;

            emit AddDialogSuccessfull(Configuration);
            //emit AddDialogSuccessfull(Dialog.Name().c_str(), Dialog.Host().c_str(), Dialog.Port());
        }
    }
}

void MainWindow::RemoveButtonPushed()
{
    QModelIndexList indexes = ui->deviceView->selectionModel()->selection().indexes();
    for(int i = 0; i < indexes.count(); ++i){
        QModelIndex index = indexes.at(i);
        QStandardItem *item = m_Model.item(index.row());
        emit RemoveDevice(item->text().toLocal8Bit().data());
    }
}

void MainWindow::StartButtonPushed()
{
    QModelIndexList indexes = ui->deviceView->selectionModel()->selection().indexes();
    for(int i = 0; i < indexes.count(); ++i){
        QModelIndex index = indexes.at(i);
        QStandardItem *item = m_Model.item(index.row());
        emit StartDevice(item->text().toLocal8Bit().data());
    }
}

void MainWindow::StopButtonPushed()
{
    QModelIndexList indexes = ui->deviceView->selectionModel()->selection().indexes();
    for(int i = 0; i < indexes.count(); ++i){
        QModelIndex index = indexes.at(i);
        QStandardItem *item = m_Model.item(index.row());
        emit StopDevice(item->text().toLocal8Bit().data());
    }
}

// public slots
void MainWindow::ClearDevices()
{
    m_Model.clear();
    QStringList Labels;
    Labels << "Name" << "Status";
    m_Model.setHorizontalHeaderLabels(Labels);
}

void MainWindow::AddDeviceToView(const QString& DeviceName, const QString& DeviceState)
{
    QStandardItem *name = new QStandardItem(DeviceName);
    QStandardItem *status = new QStandardItem(DeviceState);
    m_Model.appendRow(QList< QStandardItem* >() << name << status);
}

void MainWindow::UpdateServerStatus(const QString& ServerStatus)
{
    emit OnUpdateServerStatus(ServerStatus);
}


void MainWindow::InitFromStore(QTSettingsDeviceConfigurationStore& Store)
{
    auto DeviceConfigurations = Store.GetAllDeviceConfigurations();
    for (auto& Configuration: DeviceConfigurations)
    {
        std::string Name;
        Configuration.Get("Name", Name);
        m_DeviceConfigurations[QString::fromStdString(Name)] = Configuration;
        emit AddDialogSuccessfull(Configuration);
    }

    auto ServerConfiguration = Store.GetServerConfiguration();
    std::string DeviceType;
    if(ServerConfiguration.Get("Type", DeviceType) && DeviceType == "UDP_SERVER")
    {
        int Port = 0;
        if(ServerConfiguration.Get("Port", Port))
        {
            ui->PortSipinBox->setValue(Port);
        }
    }
}
