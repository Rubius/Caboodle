#ifndef NATIVETRANSLATES_H
#define NATIVETRANSLATES_H

#ifdef QT_C_TRANSLATES

#include <QObject>
#define TR_W(text) \
    QObject::tr(text).toStdWString()
#define TR(text) \
    QObject::tr(text)

#else

#define TR_W(text) L##text
#define TR(text) text

#endif

#endif // TRANSLATES_H
