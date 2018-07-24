#ifndef THREADFUNCTION_H
#define THREADFUNCTION_H

#include <QThread>
#include <functional>

class ThreadFunction : public QThread
{
    typedef std::function<void ()> RunFunction;
    typedef std::function<void ()> OnFinishFunction;
    ThreadFunction(const RunFunction& function, const OnFinishFunction& onFinish);
public:
    static void Async(const RunFunction& function);
    static void Async(const RunFunction& function, const OnFinishFunction& onFinish);

    // QThread interface
protected:
    void run() Q_DECL_OVERRIDE;

private:
    RunFunction _function;
    OnFinishFunction _onFinish;
};

#endif // THREADFUNCTION_H
