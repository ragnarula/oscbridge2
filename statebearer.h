#ifndef STATEBEARER_H
#define STATEBEARER_H
#include <mutex>
#include "statechangelistener.h"


template <class T>
class StateBearer
{
public:
    StateBearer(StateChangeListener<T>& Listener, T DefaultState) :
        m_Listener(Listener),
        m_CurrentState(DefaultState)

    {}

    T GetState()
    {
        return m_CurrentState;
    }

protected:
    void SetState(T State)
    {
        std::unique_lock<std::mutex> Lock(m_StateMutex);
        m_CurrentState = State;
        m_Listener.OnStateChange();
    }

private:
    StateChangeListener<T>& m_Listener;
    T m_CurrentState;
    std::mutex m_StateMutex;
};

#endif // STATEBEARER_H
