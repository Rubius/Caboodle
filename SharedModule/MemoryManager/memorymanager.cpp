#include "MemoryManager.h"
#include <QDebug>

QHash<size_t,qint32> MemoryManager::constructed;
QHash<size_t,qint32> MemoryManager::destroyed;
QHash<size_t, const char*> MemoryManager::transcription;

qint32 MemoryManager::shouldBe(size_t index)
{
    return MemoryManager::constructed[index] - MemoryManager::destroyed[index];
}

const char *MemoryManager::typeName(size_t _type)
{
    return MemoryManager::transcription[_type];
}

void MemoryManager::MakeMemoryReport()
{
    qDebug() << "----------------------------MemoryReport------------------------";
    QHashIterator<size_t,qint32> i(constructed);
    while(i.hasNext()){
        i.next();
        if(shouldBe(i.key())){
            qDebug() << typeName(i.key()) << "constructed:" << i.value() << "destructed:" << destroyed.value(i.key());
        }
    }
    qDebug() << "----------------------------------------------------------------";
}






