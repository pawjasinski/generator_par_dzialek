#include "point.h"

void Point::generatePairs()
{
    for (int i = 0; i < parcels.size() - 1; ++i)
    {
        for (int j = i + 1; j < parcels.size(); ++j)
        {
            pairsOfParcels.push_back(QPair<Parcel*, Parcel*> (parcels.at(i), parcels.at(j)));
        }
    }
}
