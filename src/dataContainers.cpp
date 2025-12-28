#include "dataContainers.h"
#include <QDebug>

bool Connection_inheritance::operator==(const Connection_inheritance& other) const 
{
    return ParentName == other.ParentName && ChildName == other.ChildName;
}

bool Connection_dependency::operator==(const Connection_dependency& other) const 
{
    return ParentName == other.ParentName && ChildName == other.ChildName;
}

QDebug operator<<(QDebug dbg, const Connection_inheritance &c)
{
    dbg.nospace() << "Inheritance(" << c.ParentName << " -> " << c.ChildName << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const Connection_dependency &c)
{
    dbg.nospace() << "Dependency(" << c.ParentName << " -> " << c.ChildName << ")";
    return dbg.space();
}