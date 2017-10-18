#ifndef DEVICECONFIGURATION_H
#define DEVICECONFIGURATION_H

#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>


namespace std {
    std::string to_string(const char* Value);
    std::string to_string(const std::string& Value);
}

class DeviceConfiguration
{
public:
    DeviceConfiguration();
    friend std::ostream& operator<<(std::ostream& Out, const DeviceConfiguration& OtherConfiguration);
    const std::map<std::string, std::string>& GetAllParams() const;

    template <typename T>
    bool Set(const std::string&Name, const T& Value)
    {
        std::string NameLower = Name;
        std::transform(NameLower.begin(), NameLower.end(), NameLower.begin(), ::tolower);
        if(m_StringParams.find(NameLower) == m_StringParams.end())
        {
            m_StringParams[NameLower] = std::to_string(Value);
            return true;
        }
        return false;
    }

    template <typename T>
    bool Get(const std::string& Name, T& OutValue) const
    {
        std::string NameLower = Name;
        std::transform(NameLower.begin(), NameLower.end(), NameLower.begin(), ::tolower);
        if(m_StringParams.find(NameLower) != m_StringParams.end())
        {
            std::istringstream iss(m_StringParams.at(NameLower));
            iss >> OutValue;
            return true;
        }
        return false;
    }

private:
    std::map<std::string, std::string> m_StringParams;
};

#endif // DEVICECONFIGURATION_H
