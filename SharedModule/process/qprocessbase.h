#ifndef QPROCESSBASE_H
#define QPROCESSBASE_H

#include <QString>

#include "processbase.h"

class QProcessBase : public ProcessBase
{
    typedef ProcessBase Super;
public:
    QProcessBase(){}

    void BeginProcess(const QString& title);
    void BeginProcess(const QString& title, int stepsCount);
    void SetProcessTitle(const QString& title);
};

#endif // QPROCESSBASE_H
