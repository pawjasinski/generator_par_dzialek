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
        if(lista.size() != 4) //bp nr x y nr dzialki - rowniez wyrzucic to do zmiennej!!!
        {
            QMessageBox::warning(this, "warning", "Something wrong in line: " + QString::number(i + 1) + " : " + temp);
            points->clear();
            fil.close();
            break;
        }
        for(int j = 0; j < lista.size(); ++j)
        {
            lista[j] = lista[j].trimmed();
        }
        points->push_back(new Point(lista));
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
        if(temp == "") continue; // igorowanie pustych linii z pliku w tym ostatniej(jesli jest)
        lista = temp.split(dzielnik); // dziele odczyt linii z pliku do Stringlist
        if (lista.size() != 3) // 3 bo nr dzialki, KW, Wlasciciel !!!!! zamiast 4 dac zmienna w klasie, zeby w razie jakichs zmian nie bylo problemow
        {
            QMessageBox::warning(this, "warning", "Something wrong in line: " + QString::number(i+1) + " : " + temp);
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
                if(lista[0] == parcels->at(j)->getNr())
                {
                    parcels->at(j)->addOwner(lista[2]);
                    addParcel = false;
                }
            }
        }
        if(addParcel) parcels->push_back(new Parcel(lista[0], lista[1], lista[2]));
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

void MainWindow::generatePairs()
{
    for (int i = 0 ; i < points->size() - 1; ++i )
    {
        int pair = 0;
        for (int j = i + 1; j < points->size(); ++j)
        {
            if(points->at(i)->getParcelNr() == points->at(j)->getParcelNr()) ++pair;
            if(pair > 1)
            {
                pairOfNumers.push_back(QPair<QString, QString>(points->at(i)->getParcelNr(), points->at(j)->getParcelNr()));
                break;
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
        QString temp;

        temp = "LP;Nr dz. ew.do których należy ustalana granica;Właściciel/ Władający;";
        temp.append("Nr księgi wieczystej lub oznaczenie innego dokumentu określającego stan prawny działek wymienionych w kolumnie 2;");
        temp.append("Osoby biorące udział w czynnościach ustalenia przebiegu granic;Data ustalenia przebiegu granic;");
        temp.append("Sposób ustalenia przebiegu granicy (nie właściwe sposoby zostały skreślone);Nr szkicu;");
        temp.append("Oświadczenie:\r\n\r\nInne oświadczenie osób biorących udział w czynnościach ustalenia przebiegu granicy Adnotacje wykonawcy");
        temp.append("i jego podpis My, niżej podpisani, oświadczamy, że granica między działkami wymienionymi w kolumnie 2, przedstawiona na szkicu do");
        temp.append("protokołu stanowiącym integralną część niniejszego protokołu, została ustalona wg naszych zgodnych wskazań.;");
        temp.append("Inne oświadczenie osób biorących udział w czynnościach ustalenia przebiegu granicy;");
        temp.append("Adnotacje wykonawcy i jego podpis");
        text << temp;
        temp.clear();
        text.flush();
        temp = "\r\n1;2;3;4;5;6;7;8;9;10;11\r\n";
        text << temp;
        temp.clear();
        text.flush();
        for(int i = 0; i < pairOfNumers.size(); ++i)
        {
            temp = QString::number(i+1);
            temp.append(";");
            temp.append(jednostkaEwid);
            temp.append(".");
            temp.append( pairOfNumers[i].first);
            temp.append(";");
            int index {0};
            for(int j = 0; j < parcels->size(); ++j)
            {
                if(parcels->at(j)->getNr() == pairOfNumers[i].first) break; // szukam pierwszego indeksu dzialki w wektorze parcels, gdzie znajduje sie jej nrKW
                index = j;
            }
            temp.append( parcels->at(index)->getNrKW() );
            temp.append(";;;");
            //temp.append("§39.1.Na podstawie zgodnych wskazań właścicieli nieruchomości\r\n §39.2. Na podstawie ostatniego spokojnego stanu posiadania na");
            //temp.append("gruncie\r\n §39.3. Na podstawie analizy map jednostkowych PZGiK oraz orzeczeń sądowych");
            temp.append(";;;;");
            text << temp;
            temp.clear();
            text.flush();

            //second pair
            temp = QString::number(i+1);
            temp.append(";");
            temp.append(jednostkaEwid);
            temp.append(".");
            temp.append( pairOfNumers[i].second);
            temp.append(";");
            index = 0;
            for(int j = 0; j < parcels->size(); ++j)
            {
                if(parcels->at(j)->getNr() == pairOfNumers[i].second) break; // szukam pierwszego indeksu dzialki w wektorze parcels, gdzie znajduje sie jej nrKW
                index = j;
            }
            temp.append( parcels->at(index)->getNrKW() );
            temp.append(";;;");
            //temp.append("§39.1.Na podstawie zgodnych wskazań właścicieli nieruchomości\r\n §39.2. Na podstawie ostatniego spokojnego stanu posiadania na");
            //temp.append("gruncie\r\n §39.3. Na podstawie analizy map jednostkowych PZGiK oraz orzeczeń sądowych");
            temp.append(";;;;");
            text <<temp;
            temp.clear();
            text.flush();
        }
        save.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    saveResult();
}
