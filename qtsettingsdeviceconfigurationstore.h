#ifndef QTSETTINGSDEVICECONFIGURATIONSTORE_H
#define QTSETTINGSDEVICECONFIGURATIONSTORE_H

#include "deviceconfigurationstore.h"
#include "serverinteractor.h"

class QTSettingsDeviceConfigurationStore : public DeviceConfigurationStore, public ServerConfigurationStore
{
public:
    QTSettingsDeviceConfigurationStore();

    // device
    void StoreDeviceConfiguration(const std::string& Name,
                                    const DeviceConfiguration& Configuration) override;
    void RemoveDeviceConfiguration(const std::string& Name) override;
    std::vector<DeviceConfiguration> GetAllDeviceConfigurations() override;

    // server
    void StoreServerConfiguration(const DeviceConfiguration& Configuration) override;
    DeviceConfiguration GetServerConfiguration() override;
};

#endif // QTSETTINGSDEVICECONFIGURATIONSTORE_H
