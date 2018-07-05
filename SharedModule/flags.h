#ifndef ATOMICFLAGS_H
#define ATOMICFLAGS_H

#include <Qt>
#include <atomic>

template<typename ValueType, typename Enum>
class Flags{
public:
    typedef Enum enum_type;

    inline qint32 ToInt() const
    {
        return _value;
    }

    constexpr Flags(qint32 i=0)
        : _value(i)
    {}
    constexpr Flags(Enum e)
        : _value(e)
    {}


    void SetFlags(qint32 value)
    {
        _value = value;
    }

    void AddFlags(qint32 flags)
    {
        _value |= flags;
    }
    void RemoveFlags(qint32 flags)
    {
        _value &= ~flags;
    }
    bool TestFlagsAll(qint32 flags) const
    {
        return (_value & flags) == flags;
    }
    bool TestFlagsAtLeastOne(qint32 flags) const
    {
        return (_value & flags);
    }

    void SwitchFlag(Enum flag)
    {
        _value ^= flag;
    }
    void AddFlag(Enum flag)
    {
        _value |= flag;
    }
    void RemoveFlag(Enum flag)
    {
        _value &= ~flag;
    }
    bool TestFlag(Enum flag) const
    {
        return _value & flag;
    }

    void ChangeFromBoolean(qint32 flags, bool flag)
    {
        flag ? AddFlags(flags) : RemoveFlags(flags);
    }

    Flags& operator |=(const Flags other)
    {
        this->_value |= other._value;
        return *this;
    }
    Flags& operator &=(const Flags other)
    {
        this->_value &= other._value;
        return *this;
    }
    Flags& operator ^=(const Flags other)
    {
        this->_value ^= other._value;
        return *this;
    }

    Flags& operator =(const qint32 e)
    {
        this->_value = e;
        return *this;
    }

    operator qint32() const
    {
        return this->_value;
    }

private:
    ValueType _value;
};

#define DECL_ATOMIC_FLAGS(flags, Enum) \
typedef Flags<std::atomic_int32_t, Enum> flags;

#define DECL_FLAGS(flags, Enum) \
typedef Flags<qint32, Enum> flags;

#endif // ATOMICFLAGS_H
