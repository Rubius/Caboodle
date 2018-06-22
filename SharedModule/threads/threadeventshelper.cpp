#include "threadeventshelper.h"
#include <QMutexLocker>

void ThreadEvent::call()
{
    _handler();
}

ThreadEventsContainer::ThreadEventsContainer()
{

}

void ThreadEventsContainer::Asynch(ThreadEvent::FEventHandler handler)
{
    QMutexLocker locker(&_eventsMutex);
    _events.push(ThreadEvent(handler));
}

void ThreadEventsContainer::ProcessEvents()
{
    QMutexLocker locker(&_eventsMutex);
    _eventsNotified = false;
    while(!_eventsNotified) { // from spurious wakeups
        _eventsProcessed.wait(&_eventsMutex);
    }
}

void ThreadEventsContainer::CallEvents()
{
    while(!_events.empty()) {
        QMutexLocker locker(&_eventsMutex); // TODO. Be carefull potencial dead lock. Use correct
        _events.front().call();
        _events.pop();
    }

    _eventsNotified = true;
    _eventsProcessed.wakeAll();
}
