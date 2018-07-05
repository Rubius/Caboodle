#ifndef PROCCESSBASE_H
#define PROCCESSBASE_H

#include <memory>

class ProcessValue;

class ProcessBase
{
public:
    template<class T, typename ... Args>
    static void CreateProccess(Args ... args) {
        (new T(args...))->startAsyncAndDestroySelfAfterFinish();
    }

protected:
    ProcessBase();
    ~ProcessBase();

    void beginProcess(const wchar_t* title);
    void beginProcess(const wchar_t* title, int stepsCount);
    void setProcessTitle(const wchar_t* title);
    void increaseProcessStepsCount(int stepsCount);
    void incrementProcess();
    bool isProcessCanceled() const;

    virtual void run();

private:
    void startAsyncAndDestroySelfAfterFinish();

private:
    std::unique_ptr<ProcessValue> _processValue;
};


#endif // PROCCESSBASE_H
