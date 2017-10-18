#include "deviceconfiguration.h"
#include <iostream>

DeviceConfiguration::DeviceConfiguration()
{

}

namespace std {
    std::string to_string(const char* Value)
    {
        return std::string(Value);
    }

    std::string to_string(const std::string& Value)
    {
        return Value;
    }
}


const std::map<std::string, std::string>& DeviceConfiguration::GetAllParams() const
{
    return m_StringParams;
}


std::ostream& operator<<(std::ostream& Out, const DeviceConfiguration& OtherConfiguration)
{
    auto& Params = OtherConfiguration.m_StringParams;
    for(auto& ParamIter : Params)
    {
        Out << ParamIter.first << ": " << ParamIter.second << " ";
    }
    return Out;
}
