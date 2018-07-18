#ifndef THREADEVENTSHELPER_H
#define THREADEVENTSHELPER_H

#include <QWaitCondition>
#include <QMutex>

#include <queue>

class ThreadEvent
{
public:
    typedef std::function<void()> FEventHandler;

    ThreadEvent() {}
    ThreadEvent(FEventHandler  handler)
        : _handler(handler)
    {}
private:
    friend class ThreadEventsContainer;
    FEventHandler _handler;

    void call();
};

class ThreadEventsContainer
{
public:
    typedef std::function<void ()> FOnPause;
    ThreadEventsContainer();

    void Pause(const FOnPause& onPause);
    void Continue();

    void Asynch(ThreadEvent::FEventHandler handler);
    void ProcessEvents();

protected:
    void callEvents();
    void callPauseableEvents();

private:
    std::queue<ThreadEvent> _events;
    QWaitCondition _eventsProcessed;
    QWaitCondition _eventsPaused;
    QMutex _eventsMutex;
    std::atomic_bool _eventsNotified;
    std::atomic_bool _isPaused;
    FOnPause _onPause;
};

#endif // THREADEVENTSHELPER_H
