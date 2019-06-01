#ifndef POINT_H
#define POINT_H
#include <QString>
#include <QList>
#include <set>
#include <QtMath>

using std::set;
class Parcel;

class Point
{
protected:
    QString name;
    double x, y;
    set<QString> parcelNr;
    QVector<Parcel*> pointersOfParcels;

public:
    Point(): name(""), x(0), y(0) {}
    Point(const QString& name, double x, double y, const QString& parcel): name(name), x(x), y(y), parcelNr(set<QString>{parcel}) {}
    Point(const QStringList& lista) : name(lista[0]), x(lista[1].toDouble()), y(lista[2].toDouble()), parcelNr(set<QString>{lista[3]}) {}

    friend double length(const Point& pointOne, const Point& pointSecond)
    {
        return qSqrt(qPow(pointOne.x - pointSecond.x, 2) + qPow(pointOne.y - pointSecond.y, 2));
    }

    QString getNumer() {return name;}

    void addParcel(const QString& numer) { parcelNr.insert(numer); }
    void addPointerOfParcel(Parcel* pointer) { pointersOfParcels.push_back(pointer); }

    int sizeOfPointersOfParcels() {return pointersOfParcels.size();}
    Parcel* getParcelPointer(int i = 0) {return pointersOfParcels.at(i);}

    friend bool operator==(const Point& p1, const Point& p2)
    {
        if(p1.name == p2.name) return true;
        return false;
    }
};

#endif // POINT_H
