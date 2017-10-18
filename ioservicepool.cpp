#include "ioservicepool.h"
#include <boost/asio.hpp>

IOServicePool::IOServicePool(boost::asio::io_service* IOService,
                              unsigned int NumberOfThreads) :
    m_IOService(IOService),
    m_NumberOfThreads(NumberOfThreads)

{

}

IOServicePool::~IOServicePool()
{
    Stop();
}

void IOServicePool::Start()
{
    m_Work = new boost::asio::io_service::work(*m_IOService);

    for(unsigned int i = 0; i < m_NumberOfThreads; ++i)
    {
        m_ThreadPool.push_back(new std::thread(&IOServicePool::Run, this));
    }
}

void IOServicePool::Stop()
{
    if(m_Work) { delete m_Work; }

    for(std::thread* Thread : m_ThreadPool)
    {
        Thread->join();
        delete Thread;
    }

    m_ThreadPool.erase(m_ThreadPool.begin(), m_ThreadPool.end());
}
void IOServicePool::Run()
{
    m_IOService->run();
}
