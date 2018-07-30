#ifndef SHARED_INTERNAL_HPP
#define SHARED_INTERNAL_HPP

#if defined __cplusplus

#ifdef NO_QT
#undef QT_C_TRANSLATES

struct DummyLogger
{
    template<typename T>
    DummyLogger& operator<<(const T&) { return *this; }
};

#define qCInfo(void) DummyLogger()
#define qCWarning(void) DummyLogger()
#define qCCritical(void) DummyLogger()

#include "process/processbase.h"
#include "nativetranslates.h"
#else
#include "array.h"
#include "stack.h"
#include "process/qprocessbase.h"
#include "process/processbase.h"
#include "threads/threadcomputingbase.h"
#include "threads/threadeventshelper.h"
#include "threads/threadfunction.h"
#include "flags.h"
#include "profile_utils.h"
#include "shared_decl.h"
#include "nativetranslates.h"
#include "name.h"
#endif

#endif

#endif // INTERNAL_H
