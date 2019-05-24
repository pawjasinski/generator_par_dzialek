#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Paweł Jasiński");
    titCol = false;
    dzielnik = ";";
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
    loadPoints();
    loadParcels();
    loadIsSelPoints();
}

void MainWindow::on_actionPreferences_triggered()
{
    DialogPreferences dialog;
    dialog.exec();
}

void MainWindow::loadPoints()
{
    QFile fil(ui->lineEditPoints->text());
    if(! fil.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "warning", "Can't open the file: " + ui->lineEditPoints->text());
        fil.close();
        return;
    }
    QTextStream str(&fil);
    QString temp;
    QStringList lista;
    for(int i = 0; !str.atEnd(); ++i)
    {
        temp.clear();
        lista.clear();
        str.flush();
        temp = str.readLine();
        if(temp == "") continue;
        lista = temp.split(dzielnik);
        if(lista.size() != 4) //bp nr x y nr dzialki - rowniez wyrzucic to do zmiennej!!!
        {
            QMessageBox::warning(this, "warning", "Something wrong in line: " + QString::number(i + 1) + " : " + temp);
            points.clear();
            fil.close();
            break;
        }
        for(int j = 0; j < lista.size(); ++j)
        {
            lista[j] = lista[j].trimmed();
        }
        points.push_back(Point(lista));
    }
}
void MainWindow::loadParcels()
{
    QFile fil(ui->lineEditParcels->text());
    if(! fil.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "warning", "Can't open the file: " + ui->lineEditParcels->text());
        fil.close();
        return;
    }
    QTextStream str(&fil);
    QString temp;
    QStringList lista;
    for(int i = 0; !str.atEnd(); ++i)
    {
        temp.clear();
        lista.clear();
        str.flush();
        temp = str.readLine();
        if(temp == "") continue; // igorowanie pustych linii z pliku w tym ostatniej(jesli jest)
        lista = temp.split(dzielnik); // dziele odczyt linii z pliku do Stringlist
        if (lista.size() != 3) // 3 bo nr dzialki, KW, Wlasciciel !!!!! zamiast 4 dac zmienna w klasie, zeby w razie jakichs zmian nie bylo problemow
        {
            QMessageBox::warning(this, "warning", "Something wrong in line: " + QString::number(i+1) + " : " + temp);
            parcels.clear();
            fil.close();
            break;
        }
        //lista.removeAll(QString(" ")); niepotrzebne
        for(int j = 0; j < lista.size() ; ++j)
        {
            lista[j] = lista[j].trimmed(); // usuwanie bialych znakow z poczatku i konca QStringa
        }
        parcels.push_back(Parcel(lista[0], lista[1], lista[2]));
    }
}

void MainWindow::loadIsSelPoints()
{
    {
        QFile fil(ui->lineEditSelPoints->text());
        if(! fil.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, "warning", "Can't open the file: " + ui->lineEditSelPoints->text());
            fil.close();
            return;
        }
        QTextStream str(&fil);
        QString temp;
        QStringList lista;
        for(int i = 0; !str.atEnd(); ++i)
        {
            temp.clear();
            lista.clear();
            str.flush();
            temp = str.readLine();
            if(temp == "") continue;
            lista = temp.split(dzielnik);
            if(lista.size() != 4) //bp nr x y nr dzialki - rowniez wyrzucic to do zmiennej!!!
            {
                QMessageBox::warning(this, "warning", "Something wrong in line: " + QString::number(i + 1) + " : " + temp);
                points.clear();
                fil.close();
                break;
            }
            for(int j = 0; j < lista.size(); ++j)
            {
                lista[j] = lista[j].trimmed();
            }
            points.push_back(Point(lista, true));
        }
    }
}
