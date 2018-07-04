#ifndef QTCUSTOMEVENTS_H
#define QTCUSTOMEVENTS_H

#include <QEvent>
#include <functional>

class QtInlineEvent : public QEvent
{
public:
    typedef std::function<void ()> Function;
    QtInlineEvent(const Function& function)
        : QEvent(QEvent::User)
        , _function(function)
    {}
    ~QtInlineEvent() {
        _function();
    }

    static void Post(const Function& function, qint32 priority = Qt::NormalEventPriority);
private:
    Function _function;
};

#endif // QTCUSTOMEVENTS_H
