#ifndef QTQSSREADER_H
#define QTQSSREADER_H

#ifndef NO_WIDGETS_INTERFACE

#include <QString>
#include "SharedModule/shared_decl.h"

class QtQSSReader
{
    QString _fileName;
    ScopedPointer<class QtObserver> _observer;
public:
    QtQSSReader();
    ~QtQSSReader();

    void SetEnableObserver(bool enable);
    void Install(const QString& mainQSSFile);

    QString ReadAll();
};

#endif

#endif // QTQSSREADER_H
