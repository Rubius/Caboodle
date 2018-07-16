#ifndef PROCCESSBASE_H
#define PROCCESSBASE_H

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

    template<class T, typename ... Args>
    static void CreateProccess(const FOnFinish& onFinish, Args ... args) {
        (new T(args...))->startAsyncAndDestroySelfAfterFinish(onFinish);
    }

protected:
    void beginProcess(const wchar_t* title) final;
    void beginProcess(const wchar_t* title, int stepsCount) final;
    void setProcessTitle(const wchar_t* title) final;
    void increaseProcessStepsCount(int stepsCount) final;
    void incrementProcess() final;
    bool isProcessCanceled() const final;

    virtual void run();

private:
    void startAsyncAndDestroySelfAfterFinish(const FOnFinish& onFinish);

private:
    std::unique_ptr<ProcessValue> _processValue;
};


#endif // PROCCESSBASE_H
