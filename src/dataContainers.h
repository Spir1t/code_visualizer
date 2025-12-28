#ifndef A49E40D6_F54F_478D_8C83_A68D0F4DE3B5
#define A49E40D6_F54F_478D_8C83_A68D0F4DE3B5

#include <QString>
#include <QSet>

struct Connection_inheritance
{
    QString ParentName;
    QString ChildName;

    bool operator==(const Connection_inheritance& other) const;
};

struct Connection_dependency
{
    QString ParentName;
    QString ChildName;

    bool operator==(const Connection_dependency& other) const;
};

QDebug operator<<(QDebug dbg, const Connection_inheritance &c);

QDebug operator<<(QDebug dbg, const Connection_dependency &c);

inline uint qHash(const Connection_inheritance &key, uint seed)
{
    return qHash(key.ParentName, seed) ^ qHash(key.ChildName, seed);
}

inline uint qHash(const Connection_dependency &key, uint seed)
{
    return qHash(key.ParentName, seed) ^ qHash(key.ChildName, seed);
}

#endif /* A49E40D6_F54F_478D_8C83_A68D0F4DE3B5 */
