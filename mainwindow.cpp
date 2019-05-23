#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Paweł Jasiński");
    titCol = false;
    ui->checkBox->setCheckState(Qt::Unchecked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "Created by Pawel Jasinski 22019", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Ok);
}

void MainWindow::on_pushButtonPathPoints_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "text files(*.txt);; any files(*.*)");
    if (! fileName.isEmpty() )
    {
        ui->lineEditPoints->clear();
        ui->lineEditPoints->setText(fileName);
    }
}

void MainWindow::on_actionexit_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Warning", "Do you want exit", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(reply == QMessageBox::Yes) QApplication::quit();
}

void MainWindow::on_pushButtonPathParcels_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "open file", QDir::currentPath(), "text files(*.txt);; any files(*)");
    if(! fileName.isEmpty())
    {
        ui->lineEditParcels->clear();
        ui->lineEditParcels->setText(fileName);
    }
}

void MainWindow::on_pushButtonPathSelPoints_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, " open file", QDir::currentPath(), "text files(*.txt);; any files(*)");
    if(! fileName.isEmpty())
    {
        ui->lineEditSelPoints->clear();
        ui->lineEditSelPoints->setText(fileName);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    ui->lineEditPoints->clear();
    ui->lineEditParcels->clear();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    this->titCol = arg1;
}

void MainWindow::on_pushButton_Generate_clicked()
{
    QString fileNamePoints = ui->lineEditPoints->text();
    QString fileNameParcels = ui->lineEditParcels->text();
    QFile Points(fileNamePoints);
    QFile Parcels(fileNameParcels);
    if(! Points.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "warning", "Can't open file " + fileNamePoints);
        Points.close();
        return;
    }
    if(! Parcels.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "warning", "Can't open file " + fileNameParcels);
        Parcels.close();
        return;
    }

}

void MainWindow::on_actionPreferences_triggered()
{
    DialogPreferences dialog;
    dialog.exec();
}
