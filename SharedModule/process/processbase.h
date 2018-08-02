#ifndef PROCCESSBASE_H
#define PROCCESSBASE_H

#include <functional>
#include <memory>

#include "iprocess.h"

class ProcessValue;

class ProcessDummy : public IProcess
{
public:
    virtual void beginProcess(const wchar_t*) final {}
    virtual void beginProcess(const wchar_t*, int) final {}
    virtual void setProcessTitle(const wchar_t*) final {}
    virtual void increaseProcessStepsCount(int) final {}
    virtual void incrementProcess() final {}
    virtual bool isProcessCanceled() const final { return false; }
};

class ProcessBase : public IProcess
{
    typedef std::function<void ()> FOnFinish;

public:
    ProcessBase();
    ~ProcessBase();

protected:
    void beginProcess(const wchar_t* title) final;
    void beginProcess(const wchar_t* title, int stepsCount) final;
    void setProcessTitle(const wchar_t* title) final;
    void increaseProcessStepsCount(int stepsCount) final;
    void incrementProcess() final;
    bool isProcessCanceled() const final;

private:
    std::unique_ptr<ProcessValue> _processValue;
};


#endif // PROCCESSBASE_H
