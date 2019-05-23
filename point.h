#ifndef POINT_H
#define POINT_H
#include <QString>

class Point
{
protected:
    QString name;
    double x, y;
    QString parcel;
    bool isSel;
public:
    Point(): name(""), x(0), y(0), parcel(""), isSel(false) {}
    Point(const QString& name, double x, double y, const QString& parcel, bool isSel): name(name), x(x), y(y), parcel(parcel), isSel(isSel) {}
};

#endif // POINT_H
