#ifndef THREADFUNCTION_H
#define THREADFUNCTION_H

#include <QThread>
#include <functional>

class ThreadFunction : public QThread
{
    typedef std::function<void ()> RunFunction;
    ThreadFunction(const RunFunction& function);
public:
    static void Async(const RunFunction& function);

    // QThread interface
protected:
    void run() Q_DECL_OVERRIDE;

private:
    RunFunction _function;
};

#endif // THREADFUNCTION_H
