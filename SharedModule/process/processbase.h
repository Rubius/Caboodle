#ifndef PROCCESSBASE_H
#define PROCCESSBASE_H

#include <memory>

class ProcessValue;

class ProcessBase
{
public:
    void Start();

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
    std::unique_ptr<ProcessValue> _processValue;
};


#endif // PROCCESSBASE_H
