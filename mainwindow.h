#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QCheckBox>
#include <QList>
#include "dialogpreferences.h"
#include "point.h"
#include "parcel.h"

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

private slots:
    void on_actionAbout_triggered();

    void on_pushButtonPathPoints_clicked();

    void on_actionexit_triggered();

    void on_pushButtonPathParcels_clicked();

    void on_actionOpen_triggered();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_Generate_clicked();

    void on_pushButtonPathSelPoints_clicked();

    void on_actionPreferences_triggered();

private:
    Ui::MainWindow *ui;
    bool titCol;
    QVector<Point> points;
    QVector<Parcel> parcels;
    QString dzielnik;
};

class Points
{
private:
public:
};

class Parcels
{
private:
public:
};


#endif // MAINWINDOW_H
