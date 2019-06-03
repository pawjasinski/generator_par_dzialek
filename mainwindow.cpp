#include "mainwindow.h"
#include "ui_mainwindow.h"






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Paweł Jasiński");
    dzielnik = "\t";
    points = new QVector<Point*>();
    parcels = new QVector<Parcel*>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "Created by Pawel Jasinski 2019\r\n506154528", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Ok);
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
        freeMemory();
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

void MainWindow::on_actionNewFile_triggered()
{
    ui->lineEditPoints->clear();
    ui->lineEditParcels->clear();
    freeMemory();
}

void MainWindow::on_pushButton_Generate_clicked()
{
    loadPoints();
    loadParcels();
    qDebug() << "load all";
    addParcelsPointerToPoints();
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
        Point pktTemp = *pkt;
        if(i > 0)
        {
            for (int j = 0; j < points->size(); ++j) //Eliminuje duplikaty punktow o tych samych numerach
            {
                if(pktTemp == *points->at(j))
                {
                    points->at(j)->addParcelNumer(lista.at(3));
                    if(pkt != nullptr) // zeby nie wrzucal punktu do tabeli jesli juz tam jest, tylko z innym numerem dzialki
                    {
                        delete pkt;
                        pkt = nullptr;
                    }
                }
            }
        }
        if(pkt != nullptr) points->push_back(pkt);
        //qDebug() << **(points->end() - 1);
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
        bool addParcel = true;
        temp.clear();
        lista.clear();
        str.flush();
        temp = str.readLine();
        if(temp == "") continue; // ignorowanie pustych linii z pliku w tym ostatniej(jesli jest)
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
                if(lista[0] == parcels->at(j)->getNr() && lista[1] == parcels->at(j)->getNrKW() ) // eliminowanie duplikatow
                {
                    if(parcels->at(j)->getOwners().contains(lista.at(2), Qt::CaseSensitivity::CaseInsensitive)) // eliminuje powtorki w "nazwach" wlascicieli
                    {
                        addParcel = false;
                    }
                    else
                    {
                        addParcel = false;
                        parcels->at(j)->addOwner(lista.at(2));
                    }
                }
            }
        }
        if(addParcel) parcels->push_back(new Parcel(lista.at(0), lista.at(2), lista.at(1)));
    }
    addParcelsPointerToPoints();
}

void MainWindow::generatePairs()
{
    for (int x = 0; x < points->size(); ++ x)
    {
        points->at(x)->generatePairs();
        QVector<QPair<Parcel*, Parcel*>> parTemp  = points->at(x)->getPairs();
        for (int y = 0; y < parTemp.size(); ++y )
        {
            if(!pary.contains(parTemp.at(y))) pary.append(parTemp.at(y));
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
        QString temp/* = QString("1%12%13%14%15%16%17%18%19%110%111\r\n").arg(dzielnik)*/;
        //text << temp;
        temp.clear();
        text.flush();
        int index = 1;
        for (int z = 0; z < pary.size(); ++z)
        {
            QStringList ownersFirstParcel {};
            QStringList ownersSecondParcel {};
            for (int q = 0; q < pary.at(z).first->getOwners().size(); ++q)
            {
                ownersFirstParcel.append(pary.at(z).first->getOwners().at(q));
            }
            for (int w = 0; w < pary.at(z).second->getOwners().size(); ++w)
            {
                ownersSecondParcel.append(pary.at(z).second->getOwners().at(w));
            }
            temp = QString::number(index) + dzielnik +  pary.at(z).first->getNr() + dzielnik + pary.at(z).first->getNrKW() + dzielnik + ownersFirstParcel.join(", " ) + "\r\n";
            temp += QString::number(index) + dzielnik + pary.at(z).second->getNr() + dzielnik + pary.at(z).second->getNrKW() + dzielnik + ownersSecondParcel.join(", ") + "\r\n";
            index++;
            text << temp;
            temp.clear();
            text.flush();
        }
        save.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    saveResult();
}

Parcel* MainWindow::findParcel(const QString& numerOfParcel)
{
    for (int i = 0 ; i < parcels->size() ; ++i)
    {
        if (numerOfParcel == parcels->at(i)->getNr()) return parcels->at(i);
    }
    return nullptr;
}

void MainWindow::freeMemory()
{
    for (int i = 0; i < points->size(); ++i) delete points->at(i);
    for (int i = 0; i < parcels->size(); ++i) delete parcels->at(i);
    delete points;
    delete parcels;
}

void MainWindow::addParcelsPointerToPoints() // after load points and parcels, important
{
    for (int i = 0; i < points->size(); ++i)
    {
        for (int j = 0; j < points->at(i)->getNumerOfParcels().size(); ++j)
        {
            QString nrParcel = points->at(i)->getNumerOfParcels().at(j);
            Parcel* par = findParcel(nrParcel);
            points->at(i)->addParcelPointer(par);
        }
    }
}
