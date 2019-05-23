#ifndef PARCEL_H
#define PARCEL_H

#include "point.h"
#include <QVector>

class Parcel
{
private:
    QString name;
    QVector<QString> owners;
    QString kw;
public:
    Parcel(): name(""), owners(QVector<QString> {}) {}
};

#endif // PARCEL_H
