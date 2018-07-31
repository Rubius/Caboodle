#ifndef NAMINGCONVENTION_H
#define NAMINGCONVENTION_H

#include <stdint.h>

typedef int32_t count_t;

#ifdef NO_QT

struct DummyLogger
{
    template<typename T>
    DummyLogger& operator<<(const T&) { return *this; }
    DummyLogger& operator<<(DummyLogger&) { return *this; }
};

#define qCInfo(void) DummyLogger()
#define qCWarning(void) DummyLogger()
#define qCCritical(void) DummyLogger()

#define Q_DECL_OVERRIDE override
#define Q_DECL_NOEXCEPT noexcept

#include <assert.h>
#define Q_ASSERT(condition) assert(condition)

#else

#include <Qt>

#endif

#endif // NAMINGCONVENTION_H
