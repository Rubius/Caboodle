#ifndef PROCCESSBASE_H
#define PROCCESSBASE_H

#include "SharedModule/internal.hpp"

class ProccessValue;

class ProccessBase
{
public:
    void Start();

protected:
    ProccessBase();
    ~ProccessBase();

    void beginProccess(const QString& title);
    void beginProccess(const QString& title, qint32 stepsCount);
    void setProccessTitle(const QString& title);
    void increaseProccessStepsCount(qint32 stepsCount);
    bool stepProccess();

    virtual void run();

private:
    ScopedPointer<ProccessValue> _proccessValue;
};

#endif // PROCCESSBASE_H
