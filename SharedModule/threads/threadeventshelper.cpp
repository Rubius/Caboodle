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
        ThreadEvent event;
        {
            QMutexLocker locker(&_eventsMutex);
            event = _events.front();
            _events.pop();
        }
        event.call();
    }

    _eventsNotified = true;
    _eventsProcessed.wakeAll();
}
