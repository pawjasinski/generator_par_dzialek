#ifndef PAIR_H
#define PAIR_H
#include "point.h"
#include "parcel.h"

class PairOfParcels
{
private:
    QPair<QString, QString> pair;
    QPair<QString, QString> nrKW;
    QVector<QString> owners;
public:
    PairOfParcels(const QPair<QString, QString>& pair, QPair<QString, QString>& nrKW, QVector<QString> owners ): pair(pair), nrKW(nrKW), owners(owners) {}
    PairOfParcels(const QString& firstOwn, const QString& nrKWfirst, QVector<QString> owner)
    {
        pair.first = firstOwn;
        nrKW.first = nrKWfirst;
        owners = owner;
    }
    PairOfParcels() {}
    QPair<QString, QString> getPair() {return pair;}
    QPair<QString, QString> getnrKW() {return nrKW;}
    QVector<QString> getOwners() {return owners;}
    void addPairNr(const QString& nr)
    { if( pair.first.isEmpty() ) pair.first = nr;
        else pair.second = nr;
    }
    void addNrKw(const QString& nrkw)
    {
        if( nrKW.first.isEmpty()) nrKW.first = nrkw;
        else nrKW.second = nrkw;
    }
    void addOwner(const QString& own) {owners.push_back(own);}
};

#endif // PAIR_H
