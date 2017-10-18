#ifndef DEVICECONFIGURATIONSTORE_H
#define DEVICECONFIGURATIONSTORE_H
#include "vector"

class DeviceConfiguration;

class DeviceConfigurationStore
{
public:
    DeviceConfigurationStore();

    virtual void StoreDeviceConfiguration(const std::string& Name,
                                    const DeviceConfiguration& Configuration) = 0;

    virtual void RemoveDeviceConfiguration(const std::string& Name) = 0;

    virtual std::vector<DeviceConfiguration> GetAllDeviceConfigurations() = 0;

};

#endif // DEVICECONFIGURATIONSTORE_H
