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
    _events.emplace_back(ThreadEvent(handler));
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
    forever {
        QMutexLocker locker(&_eventsMutex);
        if(_events.empty()) {
            break;
        }
        _events.front().call();
        _events.erase(_events.begin());
    }

    _eventsNotified = true;
    _eventsProcessed.wakeAll();
}
