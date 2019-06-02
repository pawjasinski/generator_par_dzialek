#ifndef PARCEL_H
#define PARCEL_H

#include <QVector>
#include <QDebug>

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

    QString getNrKW() {return kw;}

    QStringList getOwners() { return owner; }

    void addOwner(const QString& own) { owner.append(own); }

    friend bool operator==(const Parcel& parFirst, const Parcel& parSec)
    {
        if(parFirst.nr == parSec.nr && parFirst.owner == parSec.owner && parFirst.kw == parSec.kw) return true;
                else return false;
    }

    friend QDebug operator<<(QDebug stream, const Parcel& parcel)
    {
        stream << parcel.nr + "; " + parcel.kw + "; " + parcel.owner.join(", ");
        return stream;
    }
};

#endif // PARCEL_H
