#ifndef NAMINGCONVENTION_H
#define NAMINGCONVENTION_H

#include <stdint.h>

typedef int32_t count_t;

struct DummyLogger
{
    template<typename T>
    DummyLogger& operator<<(const T&) { return *this; }
    DummyLogger& operator<<(DummyLogger&) { return *this; }
};

#ifdef NO_QT

#define qCInfo(void) DummyLogger()
#define qCWarning(void) DummyLogger()
#define qCCritical(void) DummyLogger()

#define Q_DECL_OVERRIDE override
#define Q_DECL_NOEXCEPT noexcept

#include <assert.h>
#define Q_ASSERT(condition) assert(condition)
#define Q_ASSERT_X(condition, what, text) assert(condition)

#else

#include <Qt>

#endif

#endif // NAMINGCONVENTION_H
