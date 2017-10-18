#ifndef IOSERVICEPOOL_H
#define IOSERVICEPOOL_H

#include <stdint.h>
#include <vector>
#include <thread>
#include <boost/asio.hpp>

class IOServicePool
{
    boost::asio::io_service* m_IOService = nullptr;
    boost::asio::io_service::work* m_Work = nullptr;
    std::vector<std::thread*> m_ThreadPool;
    unsigned int m_NumberOfThreads;
public:
    IOServicePool(boost::asio::io_service* IOService, unsigned int NumberOfThreads);
    ~IOServicePool();
    void Start();
    void Stop();

private:
    void Run();
};

#endif // IOSERVICEPOOL_H
