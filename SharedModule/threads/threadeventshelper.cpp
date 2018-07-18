#include "threadeventshelper.h"
#include <QMutexLocker>

void ThreadEvent::call()
{
    _handler();
}

ThreadEventsContainer::ThreadEventsContainer()
    : _isPaused(false)
{

}

void ThreadEventsContainer::Continue()
{
    if(!_isPaused) {
        return;
    }
    _isPaused = false;
    _eventsPaused.wakeAll();
}

void ThreadEventsContainer::Pause(const FOnPause& onPause)
{
    if(_isPaused) {
        return;
    }
    _onPause = onPause;
    _isPaused = true;

    if(_events.empty()) {
        Asynch([]{});
    }

    while (!_events.empty() && _eventsMutex.tryLock()) {
        _eventsMutex.unlock();
    }
}

void ThreadEventsContainer::Asynch(ThreadEvent::FEventHandler handler)
{
    QMutexLocker locker(&_eventsMutex);
    _events.push(ThreadEvent(handler));
}

void ThreadEventsContainer::ProcessEvents()
{
    QMutexLocker locker(&_eventsMutex);
    while(!_events.empty()) { // from spurious wakeups
        _eventsProcessed.wait(&_eventsMutex);
    }
}

void ThreadEventsContainer::callEvents()
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

    _eventsProcessed.wakeAll();
}

void ThreadEventsContainer::callPauseableEvents()
{
    while(!_events.empty()) {
        ThreadEvent event;
        {
            if(_isPaused) {
                _onPause();
            }
            QMutexLocker locker(&_eventsMutex);
            event = _events.front();
            _events.pop();
            while(_isPaused) {
                _eventsPaused.wait(&_eventsMutex);
            }
        }
        event.call();
    }

    _eventsProcessed.wakeAll();
}
