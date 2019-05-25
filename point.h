#ifndef POINT_H
#define POINT_H
#include <QString>
#include <QList>
#include <QtMath>

class Point
{
protected:
    QString name;
    double x, y;
    QString parcelNr;
    bool isSel;
public:
    Point(): name(""), x(0), y(0), parcelNr(""), isSel(false) {}
    Point(const QString& name, double x, double y, const QString& parcel, bool isSel): name(name), x(x), y(y), parcelNr(parcel), isSel(isSel) {}
    Point(const QStringList& lista, bool isSel = false) : name(lista[0]), x(lista[1].toDouble()), y(lista[2].toDouble()), parcelNr(lista[3]), isSel(isSel) {}
    friend double length(const Point& pointOne, const Point& pointSecond)
    {
        return qSqrt(qPow(pointOne.x - pointSecond.x, 2) + qPow(pointOne.y - pointSecond.y, 2));
    }
    QString getParcelNr() {return parcelNr;}
};

#endif // POINT_H
