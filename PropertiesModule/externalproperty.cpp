#include "externalproperty.h"

void ExternalNamedUIntProperty::SetNames(const QStringList& names)
{
    _max = names.size() - 1;
    _names = names;
}
