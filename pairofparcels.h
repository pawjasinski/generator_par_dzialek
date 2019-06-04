#ifndef PAIROFPARCELS_H
#define PAIROFPARCELS_H
#include <QPair>
#include "parcel.h"

class PairOfParcels
{
private:
    QPair<Parcel*, Parcel*> pair;
public:
    PairOfParcels(Parcel* first, Parcel* second)
    {
        pair.first = first;
        pair.second = second;
    }
    QPair<Parcel*, Parcel*> getPair() { return pair; }

    Parcel* getFirst() { return pair.first; }
    Parcel* getSecond() { return pair.second; }

    friend bool operator==(const PairOfParcels& pairFirst, const PairOfParcels& pairSecond)
    {
        if( ( *(pairFirst.pair.first) == *(pairSecond.pair.first) && *(pairFirst.pair.second) == *(pairSecond.pair.second) ) ||\
                ( *(pairFirst.pair.first) == *(pairSecond.pair.second) && *(pairFirst.pair.second) == *(pairSecond.pair.first) )  ) return true;
        else return false;

    }
};

#endif // PAIROFPARCELS_H
