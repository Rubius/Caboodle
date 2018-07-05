#ifndef QPROCESSBASE_H
#define QPROCESSBASE_H

#include <QString>

#include "processbase.h"

class QProcessBase : public ProcessBase
{
    typedef ProcessBase Super;
protected:
    QProcessBase(){}

public:
    void beginProcess(const QString& title);
    void beginProcess(const QString& title, int stepsCount);
    void setProcessTitle(const QString& title);
};

#endif // QPROCESSBASE_H
