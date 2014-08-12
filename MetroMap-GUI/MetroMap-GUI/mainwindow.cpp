#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMap>

#define startingProgram 1
#define openXML 2
#define mapsAreUploaded 3
#define mapIsOpened 4
#define startCreatingMap 5
#define inputNumberOfAddingLines 6
#define startAddingLineNumOfSt 7
#define startAddingLineFirstSt 8
#define addingLineInputWeight 9
#define addingLineInputNameOfSt 10
#define startAddingTransfersInputNum 11
#define addingTransferInputFirstSt 12
#define addingTransferInputSecondSt 13
#define addingTransferInputWeight 14
#define addingNameOfExportXML 15
#define findFastWayInput1St 16
#define findFastWayInput2St 17
#define findMinCrossWayInput1St 18
#define findMinCrossWayInput2St 19
#define findWayThroughInp1 20
#define findWayThroughInp2 21
#define findWayThroughInp3 22
#define editingTransferInp1 23
#define editingTransferInp2 24
#define editingTransferInpWeight 25
#define deletingTrInp1 26
#define deletingTrInp2 27
#define renamingInp1 28
#define renamingInpNew 29
#define deleteStInp 30
#define deleteStInpNewWeight 31
#define addStInp1St 32
#define addStInp2St 33
#define addStInpName 34
#define addStInpWeight1 35
#define addStInpWeight2 36
#define deletingLineInpNum 37

void readNextNTimes (QXmlStreamReader &xmlReader, int n)
{
    for (int i = 0; i < n; i++)
        xmlReader.readNext();
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    state = startingProgram;
    ui->pushButton1->setText("Create new map");
    connect(ui->pushButton1, SIGNAL(clicked()), this, SLOT(slotButton1()));
    ui->pushButton2->setText("Open map in XML");
    connect(ui->pushButton2, SIGNAL(clicked()), this, SLOT(slotButton2()));
    ui->pushButtonInput->setText("Input");
    connect(ui->pushButtonInput, SIGNAL(clicked()), this, SLOT(slotButtonInput()));
    ui->pushButtonExit->setText("Exit");
    connect(ui->pushButtonExit, SIGNAL(clicked()), this, SLOT(slotButtonExit()));
    ui->textBrowserMenu->append("No available maps\n");

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotAddLine()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotDeleteLine()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(slotAddStation()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(slotDeleteStation()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(slotRenameStation()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(slotAddTransfer()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(slotDeleteTransfer()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(slotEditTransfer()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(slotFindTheWayFastest()));
    connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(slotFindTheWayMinCross()));
    connect(ui->pushButton_11, SIGNAL(clicked()), this, SLOT(slotFindTheWayThroughStation()));
    connect(ui->pushButton_12, SIGNAL(clicked()), this, SLOT(slotExportInXML()));
    connect(ui->pushButton_13, SIGNAL(clicked()), this, SLOT(slotCancel()));

    this->hideMapMenu();

    MainWindow::uploadedMaps = new QList<Map*>();
    MainWindow::numOfAddingLinesLeft = 0;
    MainWindow::numOfAddingStationsLeft = 0;
}

void MainWindow::printLine (Map* openedMap, int line)
{
    MainWindow::ui->textEditOpenedMap->append("  Stations of line " + QString::number(line) + ":");
    QMap<string, Station*>::iterator ind;
    for (ind = openedMap->stations.begin(); ind != openedMap->stations.end(); ind++)
        if (ind.value()->line == line)
        {
            MainWindow::ui->textEditOpenedMap->append("    " + QString::fromStdString(ind.value()->nameOfStation));
            QString str = "      Transfers: ";

            for (int indTransf = 0; indTransf < ind.value()->transfers.length(); indTransf++)
            {

                str += QString::fromStdString(ind.value()->transfers[indTransf]->to->nameOfStation + " (" );
                if (ind.value()->transfers[indTransf]->to->line == line)
                    str += "Neighbour, ";
                else
                    str += ("Crossline to " + QString::number(ind.value()->transfers[indTransf]->to->line) + ", ");

                str += (QString::number(ind.value()->transfers[indTransf]->weight) + " min) . ");
            }
            MainWindow::ui->textEditOpenedMap->append(str + "\n");
        }
}

void MainWindow::printMap (Map* openedMap)
{
    this->ui->textEditOpenedMap->clear();
    this->ui->textEditOpenedMap->append("OPENED MAP: " + QString::fromStdString(openedMap->nameOfMap));
    for (int i = 1; i <= openedMap->numberOfLines; i++)
        printLine(openedMap, i);
}

void MainWindow::slotButton2()
{
    switch (this->state)
    {
    case startingProgram: // "Open xml\n";
        this->state = openXML;
        ui->textBrowserMenu->append("Opening XML: input the name of file\n");
        break;
    case mapsAreUploaded:
        this->state = openXML;
        ui->textBrowserMenu->append("Opening XML: input the name of file\n");
        break;
    }
}

void MainWindow::slotButton1()
{
    switch (this->state) {
    case startingProgram: // "Create new map\n";
        this->state = startCreatingMap;
        this->createMapMode();
        break;
    case mapsAreUploaded: // "Create new map\n";
        this->state = startCreatingMap;
        this->createMapMode();
        break;
    }
}

void MainWindow::createMapMode()
{

    ui->textBrowserMenu->append("Creating map: input the name of the map\n");
    this->openedMap = new Map();
    this->openedMap->numberOfLines = 0;
    this->uploadedMaps->append(this->openedMap);
}

void MainWindow::slotButtonInput()
{
    QString inputQString = ui->lineEdit->text();
    QFile *file = new QFile();
    QFile *fileOut = new QFile();
    QXmlStreamWriter xmlWriter(fileOut);
    int numOfOpeningMap = 0;
    switch (this->state) {
    case openXML:

        file->setFileName(inputQString);
        if (file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            state = mapsAreUploaded;
            QXmlStreamReader xmlReader(file);
            while (!xmlReader.atEnd() && !xmlReader.hasError())
            {
                xmlReader.readNext();
                if (xmlReader.isStartDocument())
                    continue;
                if (xmlReader.isStartElement() && xmlReader.name() == "Map")
                {
                    Map *newMap = new Map();
                    uploadedMaps->append(newMap);
                    readNextNTimes(xmlReader, 3); //переход к nameOfMap
                    newMap->nameOfMap = xmlReader.text().toString().toStdString();
                    readNextNTimes(xmlReader, 4); //переход к numOfLines
                    newMap->numberOfLines = xmlReader.text().toInt();
                    readNextNTimes(xmlReader, 5); //переход к Station
                    while (xmlReader.name() == "Station")
                    {
                        Station *newStation = new Station();
                        readNextNTimes(xmlReader, 3);
                        newStation->nameOfStation = xmlReader.text().toString().toStdString();
                        readNextNTimes(xmlReader, 4);
                        newStation->line = xmlReader.text().toInt();
                        readNextNTimes(xmlReader, 5);
                        newMap->stations.insert(newStation->nameOfStation, newStation);
                    }
                    readNextNTimes(xmlReader, 4); //переход к первому Transfer
                    while (xmlReader.name() == "Transfer")
                    {
                        Transfer *newTransfer = new Transfer();
                        readNextNTimes(xmlReader, 3);
                        Station *stationFrom = newMap->stations.value(xmlReader.text().toString().toStdString());
                        readNextNTimes(xmlReader, 4);
                        Station *stationTo = newMap->stations.value(xmlReader.text().toString().toStdString());
                        readNextNTimes(xmlReader, 4);
                        newTransfer->to = stationTo;
                        newTransfer->weight = xmlReader.text().toInt();
                        stationFrom->transfers.append(newTransfer);
                        readNextNTimes(xmlReader, 5);
                    }
                    readNextNTimes(xmlReader, 3); //переход к следующей карте, открытие Map
                }
            }
            file->close();
            ui->textBrowserMenu->append("Succesfully uploaded\n");
            state = mapsAreUploaded;
            this->uploadedMapsMode();
        }
        else
        {
            ui->textBrowserMenu->append("Error with opening file\n");
            if (this->uploadedMaps->size() == 0)
                ui->textBrowserMenu->append("No available maps\n");
            else
            {
                state = mapsAreUploaded;
                this->uploadedMapsMode();
            }
        }

        ui->lineEdit->clear();
        break;

    case mapsAreUploaded:
        numOfOpeningMap = ui->lineEdit->text().toInt();
        ui->lineEdit->clear();
        if ((numOfOpeningMap <= 0) || (numOfOpeningMap > this->uploadedMaps->length()))
            ui->textBrowserMenu->append("Error: wrong index of map\n");
        else
        {
            this->openedMap = this->uploadedMaps->at(numOfOpeningMap - 1);
            this->state = mapIsOpened;
            this->mapIsOpenedMode();
        }
        break;

    case startCreatingMap: //name of new map
        this->ui->pushButton1->hide();
        this->ui->pushButton2->hide();
        this->openedMap->nameOfMap = ui->lineEdit->text().toStdString();
        ui->lineEdit->clear();
        ui->textBrowserMenu->append(QString::fromStdString("Name of map: " + this->openedMap->nameOfMap));
        ui->textBrowserMenu->append("Crossline transfers will be added later");
        ui->textBrowserMenu->append("Input the number of lines you are going to add (>0)\n");
        this->state = inputNumberOfAddingLines;
        break;

    case inputNumberOfAddingLines:
        this->numOfAddingLinesLeft = ui->lineEdit->text().toInt();
        ui->textBrowserMenu->append(QString::number(this->numOfAddingLinesLeft) + " lines");
        ui->lineEdit->clear();
        this->state = startAddingLineNumOfSt;
        ui->textBrowserMenu->append("Creating line " + QString::number(this->openedMap->numberOfLines + 1) + ": input the number of stations (> 1)");
        break;

    case startAddingLineNumOfSt:
        this->numOfAddingLinesLeft--;
        this->numOfAddingStationsLeft = ui->lineEdit->text().toInt();
        ui->lineEdit->clear();
        ui->textBrowserMenu->append(QString::number(this->numOfAddingStationsLeft) + " stations");
        ui->textBrowserMenu->append("Input the name of first station\n");
        this->state = startAddingLineFirstSt;
        break;

    case startAddingLineFirstSt:
        this->newStation = new Station();
        this->newStation->line = this->openedMap->numberOfLines + 1;
        this->newStation->nameOfStation = ui->lineEdit->text().toStdString();
        ui->lineEdit->clear();
        this->openedMap->stations.insert(this->newStation->nameOfStation, this->newStation);
        this->numOfAddingStationsLeft--;
        ui->textBrowserMenu->append("Added station " + QString::fromStdString(newStation->nameOfStation));
        ui->textBrowserMenu->append("Input time it takes to get to next station");
        this->state = addingLineInputWeight;
        this->prevStation = this->newStation;
        this->newStation = NULL;
        break;

    case addingLineInputWeight:
        this->newTransfer = new Transfer();
        this->prevStation->transfers.append(this->newTransfer);
        this->newTransfer->weight = ui->lineEdit->text().toInt();
        ui->lineEdit->clear();
        ui->textBrowserMenu->append(QString::number(this->newTransfer->weight) + " mins");
        this->prevTransfer = this->newTransfer;
        this->newTransfer = NULL;
        ui->textBrowserMenu->append("\nInput the name of next station\n");
        this->state = addingLineInputNameOfSt;
        break;

    case addingLineInputNameOfSt:
        this->newStation = new Station();
        this->newStation->nameOfStation = ui->lineEdit->text().toStdString();
        ui->lineEdit->clear();
        this->newStation->line = this->prevStation->line;
        this->prevTransfer->to = this->newStation;
        this->openedMap->stations.insert(this->newStation->nameOfStation, this->newStation);
        this->numOfAddingStationsLeft--;
        ui->textBrowserMenu->append("Added station " + QString::fromStdString(newStation->nameOfStation));
        this->newTransfer = new Transfer();
        this->newStation->transfers.append(this->newTransfer);
        this->newTransfer->to = this->prevStation;
        this->newTransfer->weight = this->prevTransfer->weight;
        this->prevTransfer = NULL;
        this->newTransfer = NULL;
        this->prevStation = this->newStation;
        this->newStation = NULL;

        if (this->numOfAddingStationsLeft == 0)
        {
            this->openedMap->numberOfLines++;

            if (this->numOfAddingLinesLeft == 0)
            {
                this->state = startAddingTransfersInputNum;
                ui->textBrowserMenu->append("Filling the stations is finished");
                ui->textBrowserMenu->append("Input the number of crossline transfers you're going to add\n");
            }
            else
            {
                this->state = startAddingLineNumOfSt;
                ui->textBrowserMenu->append("Creating line " + QString::number(this->openedMap->numberOfLines + 1) + ": input the number of stations (> 1)");
            }
        }
        else
        {
            ui->textBrowserMenu->append("Input time it takes to get to next station");
            this->state = addingLineInputWeight;
        }
        break;

    case startAddingTransfersInputNum:
        this->numOfAddingTransfersLeft = ui->lineEdit->text().toInt();
        ui->lineEdit->clear();
        ui->textBrowserMenu->append(QString::number(this->numOfAddingTransfersLeft) + " transfers\n");
        if (this->numOfAddingTransfersLeft == 0)
        {
            this->state = mapIsOpened;
            this->mapIsOpenedMode();
        }
        else
        {
            this->state = addingTransferInputFirstSt;
            ui->textBrowserMenu->append("Input the name of first station\n");
        }
        break;

    case addingTransferInputFirstSt:
        if (!openedMap->stations.contains(ui->lineEdit->text().toStdString()))
        {
            ui->textBrowserMenu->append("Error: no such station, try again\n");
            break;
        }
        this->newStation = this->openedMap->stations.value(ui->lineEdit->text().toStdString());
        ui->textBrowserMenu->append("Found: " + ui->lineEdit->text());
        ui->lineEdit->clear();
        this->state = addingTransferInputSecondSt;
        ui->textBrowserMenu->append("Input the name of second station\n");
        break;

    case addingTransferInputSecondSt:
        if (!openedMap->stations.contains(ui->lineEdit->text().toStdString()))
        {
            ui->textBrowserMenu->append("Error: no such station, try again\n");
            break;
        }
        this->prevStation = this->openedMap->stations.value(ui->lineEdit->text().toStdString());
        ui->textBrowserMenu->append("Found: " + ui->lineEdit->text());
        ui->lineEdit->clear();
        if (this->newStation->isConnectedWith(this->prevStation) || this->newStation->line == this->prevStation->line)
        {
            ui->textBrowserMenu->append("Error: stations are connected or have the same line\n");
            this->state = mapIsOpened;
            break;
        }
        this->state = addingTransferInputWeight;
        ui->textBrowserMenu->append("Input the time between the stations\n");
        break;

    case addingTransferInputWeight:
        this->newTransfer = new Transfer();
        this->newTransfer->to = this->newStation;
        this->newTransfer->weight = ui->lineEdit->text().toInt();
        this->prevStation->transfers.append(this->newTransfer);
        this->newTransfer = new Transfer();
        this->newTransfer->to = this->prevStation;
        this->newTransfer->weight = ui->lineEdit->text().toInt();
        ui->lineEdit->clear();
        this->newStation->transfers.append(this->newTransfer);

        this->numOfAddingTransfersLeft--;
        if (this->numOfAddingTransfersLeft == 0)
        {
            ui->textBrowserMenu->append("The last transfer is added\n");
            this->state = mapIsOpened;
            this->mapIsOpenedMode();
        }
        else
        {
            ui->textBrowserMenu->append("Transfer is added");
            this->state = addingTransferInputFirstSt;
            ui->textBrowserMenu->append("Input the name of first station\n");
        }
        break;

    case addingNameOfExportXML:
        fileOut->setFileName(inputQString);
        fileOut->open(QIODevice::WriteOnly);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("Map");

        xmlWriter.writeStartElement("nameOfMap");
        inputQString = QString::fromStdString(openedMap->nameOfMap);
        xmlWriter.writeCharacters(inputQString);
        xmlWriter.writeEndElement();//nameOfMap

        xmlWriter.writeStartElement("numOfLines");
        inputQString = QString::number(openedMap->numberOfLines);
        xmlWriter.writeCharacters(inputQString);
        xmlWriter.writeEndElement();//numOfLine

        xmlWriter.writeStartElement("Stations");

        for (QMap<string, Station*>::iterator ind = openedMap->stations.begin(); ind != openedMap->stations.end(); ind++)
        {
            xmlWriter.writeStartElement("Station");
            xmlWriter.writeStartElement("nameOfStation");
            inputQString = QString::fromStdString(ind.value()->nameOfStation);
            xmlWriter.writeCharacters(inputQString);
            xmlWriter.writeEndElement();//nameOfStation
            xmlWriter.writeStartElement("numOfLine");
            inputQString = QString::number(ind.value()->line);
            xmlWriter.writeCharacters(inputQString);
            xmlWriter.writeEndElement();//numOfLine
            xmlWriter.writeEndElement();//Station
        }
        xmlWriter.writeEndElement();//Stations

        xmlWriter.writeStartElement("Transfers");
        for (QMap<string, Station*>::iterator ind = openedMap->stations.begin(); ind != openedMap->stations.end(); ind++)
        {
            for (int i = 0; i < ind.value()->transfers.size(); i++)
            {
                xmlWriter.writeStartElement("Transfer");

                xmlWriter.writeStartElement("stationFrom");
                inputQString = QString::fromStdString(ind.value()->nameOfStation);
                xmlWriter.writeCharacters(inputQString);
                xmlWriter.writeEndElement();//stationFrom

                xmlWriter.writeStartElement("stationTo");
                inputQString = QString::fromStdString(ind.value()->transfers.at(i)->to->nameOfStation);
                xmlWriter.writeCharacters(inputQString);
                xmlWriter.writeEndElement();//stationTo

                xmlWriter.writeStartElement("weight");
                inputQString = QString::number(ind.value()->transfers.at(i)->weight);
                xmlWriter.writeCharacters(inputQString);
                xmlWriter.writeEndElement();//weight

                xmlWriter.writeEndElement();//Transfer
            }
        }
        xmlWriter.writeEndElement();//Transfers

        xmlWriter.writeEndElement();//Map
        xmlWriter.writeEndDocument();

        fileOut->close();
        ui->textBrowserMenu->append("Saved in " + ui->lineEdit->text() + "\n");
        ui->lineEdit->clear();
        this->state = mapIsOpened;
        break;

    case findFastWayInput1St:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->prevStation = openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("Station FROM: " + inputQString);
            ui->textBrowserMenu->append("Input the station TO");
            this->state = findFastWayInput2St;
        }
        break;

    case findFastWayInput2St:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->newStation = openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("Station TO: " + inputQString);

            this->openedMap->findTheWay(this->prevStation, this->newStation, ui->textBrowserMenu);
            this->prevStation = NULL;
            this->newStation = NULL;
            this->state = mapIsOpened;

        }
        break;

    case findMinCrossWayInput1St:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->prevStation = openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("Station FROM: " + inputQString);
            ui->textBrowserMenu->append("Input the station TO");
            this->state = findMinCrossWayInput2St;
        }
        break;

    case findMinCrossWayInput2St:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->newStation = openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("Station TO: " + inputQString);

            this->openedMap->findTheWayLessCrosslines(this->prevStation, this->newStation, ui->textBrowserMenu);
            this->prevStation = NULL;
            this->newStation = NULL;
            this->state = mapIsOpened;
        }
        break;

    case findWayThroughInp1:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->newStation = openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("Station FROM: " + inputQString);
            ui->textBrowserMenu->append("Input the station TO");
            this->state = findWayThroughInp2;
        }
        break;

    case findWayThroughInp2:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->prevStation = openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("Station TO: " + inputQString);
            ui->textBrowserMenu->append("Input the station THROUGH");
            this->state = findWayThroughInp3;
        }
        break;

    case findWayThroughInp3:
        if (!openedMap->stations.contains(ui->lineEdit->text().toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            ui->textBrowserMenu->append("\nJust unite two ways:\n\nWay 1:");
            this->openedMap->findTheWay(this->newStation, openedMap->stations.value(ui->lineEdit->text().toStdString()), ui->textBrowserMenu);
            ui->textBrowserMenu->append("Way 2:");
            this->openedMap->findTheWay(openedMap->stations.value(ui->lineEdit->text().toStdString()), this->prevStation, ui->textBrowserMenu);
            this->state = mapIsOpened;
        }
        ui->lineEdit->clear();
        break;

    case editingTransferInp1:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->newStation = openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("First station: " + inputQString);
            ui->textBrowserMenu->append("Input the second station\n");
            this->state = editingTransferInp2;
        }
        break;

    case editingTransferInp2:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else if (!this->openedMap->stations.value(inputQString.toStdString())->isConnectedWith(this->newStation))
        {
            ui->textBrowserMenu->append("Error: stations are not connected\n");
            this->state = mapIsOpened;
            break;
        }
        else
        {
            ui->textBrowserMenu->append("Second station: " + inputQString);
            this->prevStation = this->openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("Current time is " + QString::number(this->prevStation->getConnection(this->newStation)->weight));
            ui->textBrowserMenu->append("Imput new time\n");
            this->state = editingTransferInpWeight;
        }
        break;

    case editingTransferInpWeight:
        this->newStation->getConnection(this->prevStation)->weight = ui->lineEdit->text().toInt();
        this->prevStation->getConnection(this->newStation)->weight = ui->lineEdit->text().toInt();
        ui->textBrowserMenu->append("New weight: " + QString::number(ui->lineEdit->text().toInt()));
        ui->lineEdit->clear();
        this->state = mapIsOpened;
        this->mapIsOpenedMode();
        break;

    case deletingTrInp1:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->newStation = openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("First station: " + inputQString);
            ui->textBrowserMenu->append("Input the second station\n");
            this->state = deletingTrInp2;
        }

        break;

    case deletingTrInp2:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else if (!this->openedMap->stations.value(inputQString.toStdString())->isConnectedWith(this->newStation))
        {
            ui->textBrowserMenu->append("Error: stations are not connected\n");
            this->state = mapIsOpened;
            break;
        }
        else if (this->newStation->isNeighbour(openedMap->stations.value(inputQString.toStdString())))
        {
            ui->textBrowserMenu->append("Error: stations are on the one the same line (neighbours)\n");
            this->state = mapIsOpened;
            break;
        }
        else
        {
            this->prevStation = openedMap->stations.value(inputQString.toStdString());
            for (int i = 0; i < this->newStation->transfers.size(); i++)
                if (this->newStation->transfers.at(i)->to == this->prevStation)
                {
                    delete this->newStation->transfers.at(i);
                    this->newStation->transfers.remove(i);
                }

            for (int i = 0; i < this->prevStation->transfers.size(); i++)
                if (this->prevStation->transfers.at(i)->to == this->newStation)
                {
                    delete this->prevStation->transfers.at(i);
                    this->prevStation->transfers.remove(i);
                }
            this->state = mapIsOpened;
            this->mapIsOpenedMode();
            ui->textBrowserMenu->append("Connection is deleted\n");
        }
        break;

    case renamingInp1:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->newStation = openedMap->stations.value(inputQString.toStdString());
            ui->textBrowserMenu->append("Old name: " + inputQString);
            ui->textBrowserMenu->append("Input new name\n");
            this->state = renamingInpNew;
        }
        break;

    case renamingInpNew:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        this->newStation->nameOfStation = inputQString.toStdString();
        this->state = mapIsOpened;
        this->mapIsOpenedMode();
        break;

    case deleteStInp:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->newStation = this->openedMap->stations.value(inputQString.toStdString());
            for (QMap<string, Station*>::iterator ind = openedMap->stations.begin(); ind != openedMap->stations.end(); ind++)
                for (int i = 0; i < ind.value()->transfers.size(); i++)
                    if (ind.value()->transfers.at(i)->to == this->newStation)
                        ind.value()->transfers.remove(i);//неэффективный способ стереть все переходы на станцию

            if (openedMap->stations.value(inputQString.toStdString())->stationsIsDealEnd())
            {
                for (int i = 0; i < this->newStation->transfers.size(); i++)
                    delete this->newStation->transfers.at(i);
                this->newStation->transfers.clear();
                openedMap->stations.remove(openedMap->stations.key(this->newStation));
                delete this->newStation;
                this->state = mapIsOpened;
                this->mapIsOpenedMode();
            }
            else
            {
                Station *station2;
                Station *station3;
                bool findFirstNeighbour = false;
                for (int i = 0; i < this->newStation->transfers.size(); i++)
                {
                    if (this->newStation->transfers.at(i)->to->line == this->newStation->line)
                    {
                        if (!findFirstNeighbour)
                        {
                            station2 = this->newStation->transfers.at(i)->to;
                            findFirstNeighbour = true;
                        }
                        else
                            station3 = this->newStation->transfers.at(i)->to;
                    }
                }
                //это мы так в station2,3 записали соседей

                for (int i = 0; i < this->newStation->transfers.size(); i++)
                    delete this->newStation->transfers.at(i);
                this->newStation->transfers.clear();
                openedMap->stations.remove(openedMap->stations.key(this->newStation));
                delete this->newStation;
                //собственно сносим станцию

                this->newStation = station2;
                this->prevStation = station3;

                ui->textBrowserMenu->append(QString::fromStdString("Input new time taken to get between " + this->newStation->nameOfStation + " and " + this->prevStation->nameOfStation + "(>0)"));
                this->state = deleteStInpNewWeight;
            }
        }
        break;

    case deleteStInpNewWeight:
        if (ui->lineEdit->text().toInt() > 0)
        {
            Transfer *transfer1 = new Transfer();
            Transfer *transfer2 = new Transfer();
            transfer1->weight = ui->lineEdit->text().toInt();
            transfer2->weight = transfer1->weight;
            transfer1->to = this->newStation;
            this->prevStation->transfers.append(transfer1);
            transfer2->to = this->prevStation;
            this->newStation->transfers.append(transfer2);

            this->state = mapIsOpened;
            this->mapIsOpenedMode();
        }
        else
        {
            ui->textBrowserMenu->append("Error: weight is > 0, try again\n");
            break;
        }
        ui->lineEdit->clear();
        break;

    case addStInp1St:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->newStation = this->openedMap->stations.value(inputQString.toStdString());
            if (this->newStation->stationsIsDealEnd())
            {
                ui->textBrowserMenu->append("Station is deal-end. If you want to dispose new station behind, just press INPUT.");
                ui->textBrowserMenu->append("Otherwise, input the name of second station-neighbour. ");
            }
            else
                ui->textBrowserMenu->append("Input the name of second station-neighbour. ");
            this->state = addStInp2St;
        }
        break;

    case addStInp2St:
        inputQString = ui->lineEdit->text();
        ui->lineEdit->clear();
        if (inputQString == "")//добавление конечной
        {
            if (newStation->stationsIsDealEnd())
            {
                this->prevStation = new Station();
                prevStation->line = newStation->line;
                Transfer *tr = new Transfer();
                tr->to = newStation;
                prevStation->transfers.append(tr);
                tr = new Transfer();
                tr->to = prevStation;
                newStation->transfers.append(tr);
                ui->textBrowserMenu->append("Input the name of new adding station\n");
                this->state = addStInpName;
            }
            else
            {
                ui->textBrowserMenu->append("There is no such station: try again\n");
                break;
            }
        }
        else if (!openedMap->stations.contains(inputQString.toStdString()))
        {
            ui->textBrowserMenu->append("There is no such station: try again\n");
            break;
        }
        else
        {
            this->helpStation = this->openedMap->stations.value(inputQString.toStdString());
            if (!helpStation->isNeighbour(newStation))
            {
                ui->textBrowserMenu->append("Error: stations are not neighbours\n");
                this->state = mapIsOpened;
                break;
            }
            else//добавление посреди станций
            {
                this->prevStation = new Station();
                prevStation->line = newStation->line;

                this->helpStation = openedMap->stations.value(inputQString.toStdString());

                Transfer *tr = new Transfer();
                tr->to = prevStation;
                newStation->transfers.append(tr);
                tr = new Transfer();
                tr->to = prevStation;
                helpStation->transfers.append(tr);
                tr = new Transfer();
                tr->to = newStation;
                prevStation->transfers.append(tr);
                tr = new Transfer();
                tr->to = helpStation;
                prevStation->transfers.append(tr);
                ui->textBrowserMenu->append("Input the name of new adding station\n");
                this->state = addStInpName;
            }
        }
        break;

    case addStInpName:
        this->prevStation->nameOfStation = ui->lineEdit->text().toStdString();
        ui->textBrowserMenu->append(QString::fromStdString("Input the time between " + this->newStation->nameOfStation + " and " + this->prevStation->nameOfStation));
        ui->lineEdit->clear();
        this->state = addStInpWeight1;
        break;


    case addStInpWeight1:
        prevStation->getConnection(newStation)->weight = ui->lineEdit->text().toInt();
        newStation->getConnection(prevStation)->weight = ui->lineEdit->text().toInt();
        ui->lineEdit->clear();

        if (prevStation->stationsIsDealEnd())
        {
            openedMap->stations.insert(prevStation->nameOfStation, prevStation);
            ui->textBrowserMenu->append("Station is added\n");
            this->state = mapIsOpened;
            this->mapIsOpenedMode();
        }
        else
        {
            ui->textBrowserMenu->append(QString::fromStdString("Input the time between " + this->helpStation->nameOfStation + " and " + this->prevStation->nameOfStation));
            this->state = addStInpWeight2;
        }
        break;

    case addStInpWeight2:
        prevStation->getConnection(helpStation)->weight = ui->lineEdit->text().toInt();
        helpStation->getConnection(prevStation)->weight = ui->lineEdit->text().toInt();
        openedMap->stations.insert(prevStation->nameOfStation, prevStation);
        ui->textBrowserMenu->append("Station is added\n");
        this->state = mapIsOpened;
        this->mapIsOpenedMode();
        break;

    case deletingLineInpNum:
        if (ui->lineEdit->text().toInt() > 0 && ui->lineEdit->text().toInt() <= openedMap->numberOfLines)
        {
            int num = ui->lineEdit->text().toInt();
            for (QMap<string, Station*>::iterator ind = openedMap->stations.begin(); ind != openedMap->stations.end(); ind++)
            {
                for (int i = 0; i < ind.value()->transfers.size(); i++)
                {
                    if (ind.value()->transfers.at(i)->to->line == num)
                    {
                        delete ind.value()->transfers.at(i);
                        ind.value()->transfers.remove(i);
                        i--;
                    }
                }
            }

            for (QMap<string, Station*>::iterator ind = openedMap->stations.begin(); ind != openedMap->stations.end();)
            {
                if (ind.value()->line == num)
                {
                    for (int i = 0; i < ind.value()->transfers.size(); i++)
                        delete ind.value()->transfers.at(i);
                    //ui->textBrowserMenu->append("deleting: " + QString::fromStdString(ind.value()->nameOfStation));
                    delete ind.value();
                    ind = openedMap->stations.erase(ind);
                }
                else
                {
                    if (ind.value()->line > num)
                        ind.value()->line--;
                    ind++;
                }
            }
            openedMap->numberOfLines--;
            ui->textBrowserMenu->append("Line is deleted\n");
            state = mapIsOpened;
            mapIsOpenedMode();
        }
        else
            ui->textBrowserMenu->append("Error: try again\n");
        ui->lineEdit->clear();
        break;

    default:
        ui->textBrowserMenu->append("It's not time for input\n");
        ui->lineEdit->clear();
        break;
    }
    delete file;
    delete fileOut;
}

void MainWindow::mapIsOpenedMode()
{
    ui->textEditOpenedMap->clear();
    this->printMap(this->openedMap);
    this->ui->pushButton1->hide();
    this->ui->pushButton2->hide();

    ui->textBrowserMenu->append("Working with map...\n");

    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();
    ui->label_4->show();
    ui->pushButton->show();
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    ui->pushButton_4->show();
    ui->pushButton_5->show();
    ui->pushButton_6->show();
    ui->pushButton_7->show();
    ui->pushButton_8->show();
    ui->pushButton_9->show();
    ui->pushButton_10->show();
    ui->pushButton_11->show();
    ui->pushButton_12->show();
    ui->pushButton_13->show();
}

void MainWindow::uploadedMapsMode()
{
    ui->pushButton1->setText("Create new map");
    ui->pushButton2->setText("Open map in XML");
    this->hideMapMenu();
    ui->textBrowserMenu->append("Available maps:");
    for (int i = 0; i < uploadedMaps->length(); i++)
        ui->textBrowserMenu->append(" - " + QString::number(i + 1) + ": " + QString::fromStdString(uploadedMaps->at(i)->nameOfMap + "\n"));
    ui->textBrowserMenu->append("Input the number of the map to open it\n\n");

}

void MainWindow::slotAddLine()
{
    this->state = startAddingLineNumOfSt;
    this->numOfAddingLinesLeft = 1;
    ui->textBrowserMenu->append("Adding new line: input the number of station\n");
}

void MainWindow::slotDeleteLine()
{
    this->state = deletingLineInpNum;
    ui->textBrowserMenu->append("Deleting line: input the number of line to delete\n");
}

void MainWindow::slotAddStation()
{
    ui->textBrowserMenu->append("Adding new station: input the name of existing neighbour\n");
    this->state = addStInp1St;
}

void MainWindow::slotDeleteStation()
{
    ui->textBrowserMenu->append("Input the name of the station to delete\n");
    this->state = deleteStInp;
}

void MainWindow::slotRenameStation()
{
    ui->textBrowserMenu->append("Rename the station: input old name of station\n");
    this->state = renamingInp1;
}

void MainWindow::slotAddTransfer()
{
    this->numOfAddingTransfersLeft = 1;
    ui->textBrowserMenu->append("Adding transfer: input first station\n");
    this->state = addingTransferInputFirstSt;
}

void MainWindow::slotDeleteTransfer()
{
    ui->textBrowserMenu->append("Deleting connections: input the name of first station\n");
    this->state = deleteStInp;
}

void MainWindow::slotEditTransfer()
{
    ui->textBrowserMenu->append("Editing the weight of transfer: input first station\n");
    this->state = editingTransferInp1;
}

void MainWindow::slotFindTheWayFastest()
{
    ui->textBrowserMenu->append("Input the name of station-FROM\n");
    this->state = findFastWayInput1St;
}

void MainWindow::slotFindTheWayMinCross()
{
    ui->textBrowserMenu->append("Input the name of station-FROM\n");
    this->state = findMinCrossWayInput1St;
}

void MainWindow::slotFindTheWayThroughStation()
{
    ui->textBrowserMenu->append("Input the station FROM:");
    this->state = findWayThroughInp1;
}

void MainWindow::slotExportInXML()
{
    ui->textBrowserMenu->append("Input the name of creating file ending with .xml\n");
    this->state = addingNameOfExportXML;
}

void MainWindow::slotCancel()
{
    this->openedMap = NULL;
    this->state = mapsAreUploaded;
    this->uploadedMapsMode();
    ui->pushButton1->show();
    ui->pushButton2->show();
    ui->textEditOpenedMap->clear();
}

void MainWindow::slotButtonExit()
{
    MainWindow::close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hideMapMenu()
{
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();
    ui->pushButton_8->hide();
    ui->pushButton_9->hide();
    ui->pushButton_10->hide();
    ui->pushButton_11->hide();
    ui->pushButton_12->hide();
    ui->pushButton_13->hide();
}
