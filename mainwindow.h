#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QCheckBox>
#include <QList>
#include "dialogpreferences.h"
#include "point.h"
#include "parcel.h"
#include <algorithm>


struct comparePairOfParcels
{
    bool operator()(const QPair<Parcel*, Parcel*>& T1, const QPair<Parcel*, Parcel*>& T2)
    {
        if( T1.first->getNr() == T2.first->getNr() && T1.second->getNr() == T2.second->getNr() ) return true;
        else if( T1.first->getNr() == T2.second->getNr() && T1.second->getNr() == T1.first->getNr() ) return true;
        else return false;
    }
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadPoints();

    void loadParcels();

    void generatePairs();

    void saveResult();



private slots:
    void on_actionAbout_triggered();

    void on_pushButtonPathPoints_clicked();

    void on_actionexit_triggered();

    void on_pushButtonPathParcels_clicked();

    void on_pushButtonPathAllPoints_clicked();

    void on_actionNewFile_triggered();

    void on_pushButton_Generate_clicked();

    void on_actionPreferences_triggered();

    void on_actionSave_as_triggered();

protected:
    Ui::MainWindow *ui;
    QStringList selPoints;
    QVector<Point*> *points;
    QVector<Parcel*> *parcels;
    QVector<QPair<Parcel*, Parcel*>> pary;
    QString dzielnik;
    QString jednostkaEwid;

    Parcel* findParcel(const QString& numerOfParcel);
    void addParcelsPointerToPoints();
    void freeMemory();
    void loadSelPoints();
};


#endif // MAINWINDOW_H
