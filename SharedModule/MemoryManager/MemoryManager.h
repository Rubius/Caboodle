#ifndef TMEMORYMANAGER_H
#define TMEMORYMANAGER_H

#include <QMap>
#include <QString>
#include <typeinfo>

template<class T> class MemorySpy;

class MemoryManager {
private:
    static QMap<size_t,const char*> transcription;
    static QMap<size_t,qint32> constructed;
    static QMap<size_t,qint32> destroyed;

    static qint32 shouldBe(size_t index);
    static const char* typeName(size_t _type);
public:

    static void MakeMemoryReport();

    template<class T> friend class MemorySpy;
};

template<class T>
class MemorySpy
{
public:
    MemorySpy(){
        size_t id = typeid(T).hash_code();
        if(!MemoryManager::transcription.contains(id))
            MemoryManager::transcription[id] = typeid(T).name();
        MemoryManager::constructed[id]++;
    }

    virtual ~MemorySpy(){
        MemoryManager::destroyed[typeid(T).hash_code()]++;
    }
};

#if !defined QT_NO_DEBUG && !defined NO_QT
#define ATTACH_MEMORY_SPY(x) : MemorySpy<x>
#define ATTACH_MEMORY_SPY_2(x) , MemorySpy<x>
#else
#define ATTACH_MEMORY_SPY(x)
#define ATTACH_MEMORY_SPY_2(x)
#endif

#endif // TMEMORYMANAGER_H
