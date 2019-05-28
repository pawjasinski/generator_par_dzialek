#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Paweł Jasiński");
    titCol = false;
    dzielnik = "\t";
    ui->checkBox->setCheckState(Qt::Unchecked);
    points = new QVector<Point*>();
    parcels = new QVector<Parcel*>();
    pairOfParcels = new QVector< QPair<Parcel*, Parcel*> >();
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
    if(reply == QMessageBox::Yes)
    {
        for (int i = 0; i < points->size(); ++i) delete points->at(i);
        for (int i = 0; i < parcels->size(); ++i) delete parcels->at(i);
        delete pairOfParcels;
        delete points;
        delete parcels;
        QApplication::quit();
    }
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
    //loadIsSelPoints(); // niepotrzebne
    qDebug() << "Zaladowano all";
    generatePairs();
    QMessageBox::information(this, "Done", "Done");
}

void MainWindow::on_actionPreferences_triggered()
{
    DialogPreferences dialog;
    dialog.exec();
}
//these 3 functions below are very similar
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
        if(lista.size() != 4) //bo nr x y nr dzialki - rowniez wyrzucic to do zmiennej!!!
        {
            QMessageBox::warning(this, "warning", "Something wrong in Points in line: " + QString::number(i + 1) + " : " + temp);
            points->clear();
            fil.close();
            break;
        }
        for(int j = 0; j < lista.size(); ++j)
        {
            lista[j] = lista[j].trimmed();
        }
        Point* pkt = new Point(lista);
        for (int j = 0; j < points->size(); ++j) // sprawdzam czy nie ma powtorzonych punktow
        {
            if(pkt == points->at(j))
            {
                delete pkt;
                break;
            }
        }
        points->push_back(pkt);
    }
//    qDebug() << "loadPoints";
//    for (int i = 0; i < points->size(); ++ i)
//    {
//        qDebug() << QString::number(i) + ": " + points->at(i)->getParcelNr();
//    }
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
        bool addParcel = true;
        temp.clear();
        lista.clear();
        str.flush();
        temp = str.readLine();
        if(temp == "") continue; // igorowanie pustych linii z pliku w tym ostatniej(jesli jest)
        lista = temp.split(dzielnik); // dziele odczyt linii z pliku do Stringlist
        if (lista.size() != 3) // 3 bo nr dzialki, KW, Wlasciciel !!!!! zamiast 4 dac zmienna w klasie, zeby w razie jakichs zmian nie bylo problemow
        {
            QMessageBox::warning(this, "warning", "Something wrong in Parcels in line: " + QString::number(i+1) + " : " + temp);
            parcels->clear();
            fil.close();
            break;
        }
        //lista.removeAll(QString(" ")); niepotrzebne
        for(int j = 0; j < lista.size() ; ++j)
        {
            lista[j] = lista[j].trimmed(); // usuwanie bialych znakow z poczatku i konca QStringa
        }
        if(i > 0)//oprocz pierwszego elementu. Kod ponizej ma wyeliminowac powtorki nr dzialek(nr dzialki ma byc unikalny), dzialka moze miec wielu wlascicieli
        {
            for (int j = 0; j < parcels->size(); ++j)
            {
                if(lista[0] == parcels->at(j)->getNr() && lista[1] == parcels->at(j)->getNrKW() && ! ( parcels->at(j)->getOwners().contains(lista[2]) ) ) // eliminowanie duplikatow
                {
                    parcels->at(j)->addOwner(lista[2]);
                    addParcel = false;
                }
            }
        }
        if(addParcel) parcels->push_back(new Parcel(lista[0], lista[1], lista[2]));
    }
//    for (int i = 0 ; i < parcels->size() ; ++i)
//    {
//        qDebug() << QString::number(i) + "  " + parcels->at(i)->getNr() + " " + parcels->at(i)->getOwners().at(0);
//    }
}

void MainWindow::generatePairs()
{
    for (int i = 0 ; i < points->size() - 1 ; ++i)
    {
        QString nr = points->at(i)->getParcelNr();
        int index = 0;
        for (int j = i +1 ; j < points->size() ; ++j)
        {
            if(nr == points->at(j)->getParcelNr()) index++;
            if(index > 1)
            {
                Parcel* first = findParcel(points->at(i)->getParcelNr());
                Parcel* second = findParcel(points->at(j)->getParcelNr());
                QPair<Parcel*,Parcel*> par(first, second);
                pairOfParcels->push_back(par);
            }
        }
    }
}
void MainWindow::saveResult()
{
        QString pathToFile;
        pathToFile = QFileDialog::getSaveFileName(this, "Save file", QDir::currentPath(), "text files(*.txt);; any files(*)" );
        QFile save(pathToFile);
        save.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream text(&save);
        QString temp = "1;2;3;4;5;6;7;8;9;10;11\r\n";
        text << temp;
        temp.clear();
        text.flush();
        for (int i = 0 ; i < pairOfParcels->size() ; ++i)
        {
            temp.append(QString::number(i));
            temp.append(";");
            foreach(const QString x, pairOfParcels->at(i).first->getOwners())
            {
                temp.append(x);
                temp.append(" ");
            }
            temp.append(";;;§39.1.Na podstawie zgodnych wskazań właścicieli nieruchomości §39.2. Na podstawie ostatniego spokojnego /"
                        "stanu posiadania na gruncie §39.3. Na podstawie analizy map jednostkowych PZGiK oraz orzeczeń sądowych;;;;\r\n");

            temp.append(QString::number(i));
            temp.append(";");
            foreach(const QString x, pairOfParcels->at(i).second->getOwners())
            {
                temp.append(x);
                temp.append(" ");
            }
            temp.append(";;;§39.1.Na podstawie zgodnych wskazań właścicieli nieruchomości §39.2. Na podstawie ostatniego spokojnego /"
                        "stanu posiadania na gruncie §39.3. Na podstawie analizy map jednostkowych PZGiK oraz orzeczeń sądowych;;;;\r\n");
        }


        save.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    saveResult();
}

QString MainWindow::findNrKw(const QString& numerOfParcel)
{
    for (int i = 0; i < parcels->size(); ++i)
    {
        if(numerOfParcel == parcels->at(i)->getNr()) return parcels->at(i)->getNrKW();
    }
    return "Brak_NR_KW";
}

Parcel* MainWindow::findParcel(const QString& numerOfParcel)
{
    for (int i = 0 ; i < parcels->size() ; ++i)
    {
        if (numerOfParcel == parcels->at(i)->getNr()) return parcels->at(i);
    }
    return nullptr;
}

void MainWindow::loadIsSelPoints()// Niepotrzebne
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
                QMessageBox::warning(this, "warning", "Something wrong in selected Points in line: " + QString::number(i + 1) + " : " + temp);
                points->clear();
                fil.close();
                break;
            }
            for(int j = 0; j < lista.size(); ++j)
            {
                lista[j] = lista[j].trimmed();
            }
            points->push_back(new Point(lista, true));
        }
    }
}
