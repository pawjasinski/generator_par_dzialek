#ifndef PARCEL_H
#define PARCEL_H

#include "point.h"
#include <QVector>

class Parcel
{
private:
    QString nr;
    QString owner;
    QString kw;
public:
    Parcel(): nr("_"), owner("_"), kw("_") {}
    Parcel(QString nr,QString owner , QString kw): nr(nr), owner(owner), kw(kw) {}
    Parcel(const QStringList& list) {nr = list[0]; kw = list[1]; owner = list[2];}
};

#endif // PARCEL_H
