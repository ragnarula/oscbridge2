#ifndef STATECHANGELISTENER_H
#define STATECHANGELISTENER_H

template <class T>
class StateChangeListener
{
public:
    virtual void OnStateChange() = 0;
};

#endif // STATECHANGELISTENER_H
