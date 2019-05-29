#ifndef PARCEL_H
#define PARCEL_H

#include "point.h"
#include <QVector>

class Parcel
{
private:
    QString nr;
    QStringList owner;
    QString kw;
public:
    Parcel(): nr("_"), owner("_"), kw("_") {}
    Parcel(QString nr,QString Owner , QString kw): nr(nr), owner(Owner), kw(kw) {}
    Parcel(const QStringList& list) {nr = list[0]; kw = list[1]; owner.append(list[2]);}
    QString getNr() { return nr; }
    void addOwner(const QString& own) { owner.append(own); }
    QString getNrKW() {return kw;}
    QStringList getOwners() { return owner; }
};

#endif // PARCEL_H
