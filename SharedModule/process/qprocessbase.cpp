#include "qprocessbase.h"

void QProcessBase::beginProcess(const QString& title)
{
    Super::beginProcess(title.toStdWString().c_str());
}

void QProcessBase::beginProcess(const QString& title, int stepsCount)
{
    Super::beginProcess(title.toStdWString().c_str(), stepsCount);
}

void QProcessBase::setProcessTitle(const QString& title)
{
    Super::setProcessTitle(title.toStdWString().c_str());
}
