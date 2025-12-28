#ifndef C7720108_BF54_4C31_88BE_8F30CF15088E
#define C7720108_BF54_4C31_88BE_8F30CF15088E

#include "dataContainers.h"
#include <QSet>
#include <QString>

class Parser
{
public:
    Parser() = default;
    bool parseFile(const QString& path);

    const QSet<QString>& classes() {return mClasses;}
    const QSet<Connection_inheritance>& inheritances() {return mInheritances;}
    const QSet<Connection_dependency>& dependencies() {return mDependencies;}

private:
    QSet<QString> mClasses;
    QSet<Connection_inheritance> mInheritances;
    QSet<Connection_dependency> mDependencies;
};

#endif /* C7720108_BF54_4C31_88BE_8F30CF15088E */
