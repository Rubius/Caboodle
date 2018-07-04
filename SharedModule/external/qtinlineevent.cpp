#include "qtinlineevent.h"
#include <QCoreApplication>

void QtInlineEvent::Post(const QtInlineEvent::Function& function, Qt::EventPriority priority)
{
    QCoreApplication::postEvent(QCoreApplication::instance(), new QtInlineEvent(function), priority);
}


