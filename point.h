#ifndef POINT_H
#define POINT_H
#include <QString>
#include <QList>
#include <set>
#include <QtMath>
#include <QDebug>
#include "parcel.h"

using std::set;
class Parcel;

class Point
{
protected:
    QString name;
    double x, y;
    QStringList numerOfParcels;
    QVector<Parcel*> parcels;

public:
    Point(): name(""), x(0), y(0) {}

    Point(const QString& name, double x, double y): name(name), x(x), y(y) {}

    Point(const QStringList& lista) : name(lista[0]), x(lista[1].toDouble()), y(lista[2].toDouble()) { numerOfParcels.append(lista[3]); }

    void addParcelPointer(Parcel* par)
    {
       if(! parcels.contains(par))
       {
           parcels.push_back(par);
       }
    }

    void addParcelNumer(const QString& nr)
    {
        if( numerOfParcels.contains(nr)) numerOfParcels.push_back(nr);
    }

    QStringList getNumerOfParcels() { return numerOfParcels; }

    Parcel* getParcelPointer(int index) {return parcels.at(index); }


    friend double length(const Point& pointOne, const Point& pointSecond)
    {
        return qSqrt(qPow(pointOne.x - pointSecond.x, 2) + qPow(pointOne.y - pointSecond.y, 2));
    }

    friend bool operator==(const Point& p1,const Point& p2)
    {
        if(p1.name == p2.name) return true;
        else return false;
    }

    friend QDebug operator<<(QDebug stream, const Point& pkt)
    {
        stream << pkt.name + "; " + pkt.numerOfParcels.join(", ");
        return stream;
    }


};

#endif // POINT_H
