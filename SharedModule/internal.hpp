#ifndef SHARED_INTERNAL_HPP
#define SHARED_INTERNAL_HPP

#if defined __cplusplus

#ifdef NO_QT
#undef QT_C_TRANSLATES

#include "smartpointersadapters.h"
#include "process/processbase.h"
#include "nativetranslates.h"
#include "serialization/StreamBuffer.h"
#include "serialization/stdserializer.h"
#include "array.h"
#include "stack.h"

#else

#include "smartpointersadapters.h"
#include "serialization/StreamBuffer.h"
#include "serialization/stdserializer.h"
#include "serialization/qserializer.h"
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
