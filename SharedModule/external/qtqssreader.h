#ifndef QTQSSREADER_H
#define QTQSSREADER_H

#ifndef NO_WIDGETS_INTERFACE

#include <QString>

class QtQSSReader
{
    QString _fileName;
public:
    QtQSSReader(const QString& mainQSSFile);

    static void Install(const QString* mainQSSFile);
    static void InstallAndObserve(const QString* mainQSSFile);

    QString ReadAll() const;
};

#endif

#endif // QTQSSREADER_H
