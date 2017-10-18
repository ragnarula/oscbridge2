#include "qtsettingsdeviceconfigurationstore.h"

#include "deviceconfiguration.h"
#include <QSettings>
#include <QString>
#include <QDataStream>
#include <QVariant>
#include <QMetaType>
#include <iostream>


Q_DECLARE_METATYPE(DeviceConfiguration)


QDataStream& operator<<(QDataStream& Out, const DeviceConfiguration& Configuration)
{
    auto Params = Configuration.GetAllParams();
    Out << (int) Params.size();
    for(auto& Iterator : Params)
    {
        Out << QString::fromStdString(Iterator.first);
        Out << QString::fromStdString(Iterator.second);
    }
    return Out;
}

QDataStream& operator>>(QDataStream& In, DeviceConfiguration& Configuration)
{
    int Size;
    In >> Size;

    for(int i = 0; i < Size; ++i)
    {
        QString Name;
        QString Value;
        In >> Name;
        In >> Value;
        Configuration.Set(Name.toStdString(), Value.toStdString());
    }

    return In;
}

QTSettingsDeviceConfigurationStore::QTSettingsDeviceConfigurationStore()
{

}


void QTSettingsDeviceConfigurationStore::StoreDeviceConfiguration(const std::string& Name,
                                const DeviceConfiguration& Configuration)
{
    qRegisterMetaTypeStreamOperators<DeviceConfiguration>("DeviceConfiguration");
    QSettings settings;
    settings.beginGroup(QString("Devices"));
    settings.setValue(QString::fromStdString(Name), QVariant::fromValue(Configuration));
    settings.endGroup();

}

void QTSettingsDeviceConfigurationStore::RemoveDeviceConfiguration(const std::string& Name)
{
    QSettings settings;
    settings.beginGroup(QString("Devices"));
    settings.remove(QString::fromStdString(Name));
    settings.endGroup();
}

std::vector<DeviceConfiguration> QTSettingsDeviceConfigurationStore::GetAllDeviceConfigurations()
{
    qRegisterMetaTypeStreamOperators<DeviceConfiguration>("DeviceConfiguration");
    QSettings Settings;

    Settings.beginGroup("Devices");
    auto Keys = Settings.childKeys();

    std::vector<DeviceConfiguration> Configurations;

    for(auto& Key : Keys)
    {
        QVariant Value = Settings.value(Key);
        DeviceConfiguration C = Value.value<DeviceConfiguration>();
        Configurations.push_back(C);
    }

    Settings.endGroup();

    return Configurations;
}

void QTSettingsDeviceConfigurationStore::StoreServerConfiguration(const DeviceConfiguration& Configuration)
{
    qRegisterMetaTypeStreamOperators<DeviceConfiguration>("DeviceConfiguration");
    QSettings settings;
    settings.setValue(QString("Server"), QVariant::fromValue(Configuration));
}

DeviceConfiguration QTSettingsDeviceConfigurationStore::GetServerConfiguration()
{
    qRegisterMetaTypeStreamOperators<DeviceConfiguration>("DeviceConfiguration");
    QSettings Settings;

    auto Default = QVariant::fromValue(DeviceConfiguration());
    QVariant Value = Settings.value(QString("Server"), Default);

    return Value.value<DeviceConfiguration>();
}
