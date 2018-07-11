#ifndef NATIVETRANSLATES_H
#define NATIVETRANSLATES_H

#ifdef QT_C_TRANSLATES
#include <QObject>
#define TR_W(text) \
    QObject::tr(text).toStdWString()
#else
#define TR_W(text) L##text
#endif

#endif // TRANSLATES_H
