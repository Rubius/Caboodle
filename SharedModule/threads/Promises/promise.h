#ifndef PROMISE_H
#define PROMISE_H

#include <functional>
#include <SharedModule/smartpointersadapters.h>
#include <atomic>

template<class T>
class PromiseData
{
    template<class T> friend class Promise;
    std::function<void (const T&)> PromiseCallback;
    std::function<void (const std::exception& exception)> ErrorCallback;
    std::atomic_bool IsResolved;

    PromiseData()
        : PromiseCallback([](const T&){})
        , ErrorCallback([](const std::exception&){})
        , IsResolved(false)
    {}
};

template<class T>
class Promise
{
    SharedPointer<PromiseData<T>> m_data;
public:
    Promise()
        : m_data(new PromiseData<T>())
    {}
    Promise(const std::function<void (T)>& value)
        : m_data(new PromiseData<T>())
    {
        m_data->PromiseCallback = value;
    }

    void Resolve(const T& value)
    {
        m_data->PromiseCallback(value);
        m_data->IsResolved = true;
    }

    void Reject()
    {
        m_data->ErrorCallback(std::runtime_error("error during computing"));
    }

private:
    template<class T> friend class PromiseDelegate;
    void then(const std::function<void (T)>& then)
    {
        m_data->PromiseCallback = then;
    }

    void catch_(const std::function<void (const std::exception& exception)>& catchHandler)
    {
        m_data->ErrorCallback = catchHandler;
    }
};

template<class T>
class PromiseDelegate
{
    Promise<T>* m_promise;
public:
    PromiseDelegate(Promise<T>* promise)
        : m_promise(promise)
    {}

    void Then(const std::function<void (T)>& then)
    {
        if(m_promise != nullptr) {
            m_promise->then(then);
        }
    }

    PromiseDelegate& Catch(const std::function<void (const std::exception& exception)>& catchHandler)
    {
        if(m_promise != nullptr) {
            m_promise->catch_(catchHandler);
        } else {
            catchHandler(std::runtime_error("task was not started"));
        }
        return *this;
    }
};

typedef Promise<bool> AsyncResult;
typedef PromiseDelegate<bool> HandleAsyncResult;

#endif // PROMISE_H
