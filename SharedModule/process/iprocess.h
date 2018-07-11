#ifndef IPROCESS_H
#define IPROCESS_H

class IProcess
{
public:
    virtual ~IProcess() {}

    virtual void beginProcess(const wchar_t*) = 0;
    virtual void beginProcess(const wchar_t*, int) = 0;
    virtual void setProcessTitle(const wchar_t*) = 0;
    virtual void increaseProcessStepsCount(int) = 0;
    virtual void incrementProcess() = 0;
    virtual bool isProcessCanceled() const = 0;
};

#endif // IPROCESS_H
