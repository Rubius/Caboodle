#include "qprocessbase.h"

void QProcessBase::BeginProcess(const QString& title)
{
    Super::BeginProcess(title.toStdWString().c_str());
}

void QProcessBase::BeginProcess(const QString& title, int stepsCount)
{
    Super::BeginProcess(title.toStdWString().c_str(), stepsCount);
}

void QProcessBase::SetProcessTitle(const QString& title)
{
    Super::SetProcessTitle(title.toStdWString().c_str());
}
