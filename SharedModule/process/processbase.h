#ifndef PROCCESSBASE_H
#define PROCCESSBASE_H

#include "SharedModule/internal.hpp"

class ProcessValue;

class ProcessBase
{
public:
    void Start();

protected:
    ProcessBase();
    ~ProcessBase();

    void beginProcess(const QString& title);
    void beginProcess(const QString& title, qint32 stepsCount);
    void setProcessTitle(const QString& title);
    void increaseProcessStepsCount(qint32 stepsCount);
    bool stepProcess();

    virtual void run();

private:
    ScopedPointer<ProcessValue> _processValue;
};

#endif // PROCCESSBASE_H
