#ifndef POINT_H
#define POINT_H
#include <QString>
#include <QList>
#include <set>
#include <QtMath>
#include <QDebug>
#include <QPair>
#include "parcel.h"
#include "pairofparcels.h"

using std::set;
class Parcel;

class Point
{
protected:
    QString name;
    double x, y;
    QStringList numerOfParcels;
    QVector<Parcel*> parcels;
    QVector<QPair<Parcel*, Parcel*>*> pairsOfParcels;
    bool isSel;

public:
    Point(): name(""), x(0), y(0) {}

    Point(const QString& name, double x, double y): name(name), x(x), y(y), isSel(false) {}

    Point(const QStringList& lista) : name(lista[0]), x(lista[1].toDouble()), y(lista[2].toDouble()), isSel(false) { numerOfParcels.append(lista[3]); }

    ~Point();

    void addParcelPointer(Parcel* par)
    {
       if(! parcels.contains(par)) parcels.push_back(par);
    }

    void addParcelNumer(const QString& nr)
    {
        if( !numerOfParcels.contains(nr)) numerOfParcels.push_back(nr);
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
        QStringList show;
        for (int x = 0; x < pkt.parcels.size(); ++x)
        {
            show.append(pkt.parcels.at(x)->getNr());
        }
        stream << pkt.name/* + " " + show.join(", ")*/;
        return stream;
    }

    QVector<QPair<Parcel*, Parcel*>*> getPairs() { return  pairsOfParcels; }

    void generatePairs();
};

#endif // POINT_H
