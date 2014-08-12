#include "Station.h"
#include "transfer.h"
#include "Way.h"
#include <QMap>
 #include <QTextBrowser>

#pragma once

using namespace std;

class Map
{
public:
    //Добавление новой станции после конечнной
    Station* addStationAfter (Station *prevStation)
    {
        Station *newStation = new Station();
        newStation->line = prevStation->line;
        int newWeight = 0;
        cout << "Input the average time taken to get next station: ";
        cin >> newWeight;

        Transfer *transferToPrev = new Transfer();
        transferToPrev->to = prevStation;
        transferToPrev->weight = newWeight;
        newStation->transfers.append(transferToPrev);

        Transfer *transferFromPrev = new Transfer();
        transferFromPrev->to = newStation;
        transferFromPrev->weight = newWeight;
        prevStation->transfers.append(transferFromPrev);

        cout << "Input the name of next station: ";
        cin >> newStation->nameOfStation;
        this->stations.insert(newStation->nameOfStation, newStation);
        return newStation;
    }


    void addingLine(int numOfExistingLines)
    {
        int i = numOfExistingLines + 1;

        cout << "Filling line " << i << ": input the number of stations: ";
        int numOfStations = 0;
        cin >> numOfStations;

        //Первая станция запихивается без ссылок на соседние станции, поэтому вне цикла
        Station *newStation = new Station();
        cout << "Input the name of first station: ";
        cin >> newStation->nameOfStation;
        newStation->line = i;
        this->stations.insert(newStation->nameOfStation, newStation);

        Station *prevStation = newStation;
        for (int j = 1; j < numOfStations; j++)
        {

            prevStation = this->addStationAfter(prevStation);
        }
        cout << "Filling line " << i << " finished\n\n";
    }

    void fillMap()
    {
        cout << "Input the name of new map: ";
        cin >> this->nameOfMap;
        cout << "The mask of line is: STATION + [WEIGHT + STATION]\n";
        cout << "Transfers will be added later\n";
        cout << "\nInput the number of lines you are going to add: ";
        cin >> this->numberOfLines;
        for (int i = 0; i < this->numberOfLines; i++)
        {
            addingLine(i);
        }

        cout << "Filling the stations of map " << this->nameOfMap << " finished\n\n";

        cout << "It's time to create transfers between the lines\n";
        cout << "Input the number of crossline transfers you're going to add: ";
        int numOfTransfers = 0;
        cin >> numOfTransfers;
        for (int i = 0; i < numOfTransfers; i++)
        {
            string nameOfStation1 = "";
            string nameOfStation2 = "";
            cout << "Input the names of stations you're going to connect\n";
            cin >> nameOfStation1;
            Station *station1 = this->stations[nameOfStation1];
            cin >> nameOfStation2;
            Station *station2 = this->stations[nameOfStation2];
            cout << "Input the time taken to transfer between the stations\n";
            int transferWeight = 0;
            cin >> transferWeight;
            Transfer *transferTo1 = new Transfer();
            Transfer *transferTo2 = new Transfer();
            transferTo1->weight = transferWeight;
            transferTo2->weight = transferWeight;
            transferTo1->to = station1;
            transferTo2->to = station2;
            station1->transfers.append(transferTo2);
            station2->transfers.append(transferTo1);
            cout << "Transfer beetween " << nameOfStation1 << " and " << nameOfStation2 << " with weight " << transferWeight << " is created\n";
        }
        cout << "\n";
    }

    void findTheWayLessCrosslines (Station *stationFrom, Station *stationTo, QTextBrowser *textBrowserMenu)
    {
        Way *way = new Way();
        way->stations.append(stationFrom);
        way->numOfCrosslines = 0;
        way->totalweight = 0;
        QVector<Way*> *analysedWays = new QVector<Way*>;
        QVector<Way*>  *newWays = new QVector<Way*>;
        analysedWays->append(way);

        bool someWayIsFound = false;
        bool noWaysMore = false;

        Way *firstWay = new Way();
        firstWay->numOfCrosslines = INT_MAX;
        while (!someWayIsFound && !noWaysMore)
        {
            noWaysMore = true;
            for (int indWay = 0; indWay < analysedWays->size(); indWay++)
            {
                for (int indTransfer = 0; indTransfer < analysedWays->at(indWay)->stations.last()->transfers.size(); indTransfer++)
                {
                    if (!analysedWays->at(indWay)->stationContains(analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to))
                    {
                        Way *newWay = new Way(*analysedWays->at(indWay));

                        noWaysMore = false;

                        if (analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to->line != analysedWays->at(indWay)->stations.last()->line)
                            newWay->numOfCrosslines += 1;

                        newWay->totalweight += analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->weight;

                        newWay->stations.append(analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to);

                        if (analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to == stationTo && newWay->numOfCrosslines < firstWay->numOfCrosslines)
                        {
                            someWayIsFound = true;
                            firstWay = newWay;
                        }
                        else
                            newWays->append(newWay);
                    }
                }
            }
            //            cout << "BEFORE CHOOSING firstWay - Alternative unfinished ways (newWays) " << newWays->size() << "\n";
            //            for (int i = 0; i < newWays->size(); i++)
            //            newWays->at(i)->printWay();

            analysedWays = new QVector<Way*>(*newWays);
            newWays->clear();

            //            cout << "BEFORE CHOOSING firstWay - Alternative unfinished ways (analysed) " << analysedWays->size() << "\n";
            //            for (int i = 0; i < analysedWays->size(); i++)
            //            analysedWays->at(i)->printWay();
        }

        if (!someWayIsFound)
            textBrowserMenu->append("There is no way between the stations\n");
        else
        {
            //            cout << "Firstly found way:\n";
            //            firstWay->printWay();
            //            cout << "AFTER CHOOSING firstWay - Alternative unfinished ways\n";
            for (int i = 0; i < analysedWays->size(); i++)
            {
                //analysedWays->at(i)->printWay();
                if (analysedWays->at(i)->totalweight >= firstWay->totalweight)
                {
                    delete analysedWays->at(i);
                    analysedWays->remove(i);
                }
            }
            //            for (int i = 0; i < analysedWays->size(); i++)
            //                analysedWays->at(i)->printWay();

            while (analysedWays->size() > 0 && !noWaysMore)
            {
                noWaysMore = true;
                for (int indWay = 0; indWay < analysedWays->size(); indWay++)
                {
                    for (int indTransfer = 0; indTransfer < analysedWays->at(indWay)->stations.last()->transfers.size(); indTransfer++)
                    {
                        if (!analysedWays->at(indWay)->stationContains(analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to))
                        {
                            Way *newWay = new Way(*analysedWays->at(indWay));
                            noWaysMore = false;

                            if (analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to->line != analysedWays->at(indWay)->stations.last()->line)
                                newWay->numOfCrosslines += 1;

                            newWay->totalweight += analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->weight;

                            newWay->stations.append(analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to);

                            if (analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to == stationTo)
                            {
                                if (newWay->numOfCrosslines < firstWay->numOfCrosslines)
                                    firstWay = newWay;
                            }
                            else
                            {
                                newWays->append(newWay);
                            }
                        }
                    }
                }

                for (int i = 0; i < analysedWays->size(); i++)
                    delete analysedWays->at(i);

                delete analysedWays;
                analysedWays = new QVector<Way*>(*newWays);
                newWays->clear();


                for (int i = 0; i < analysedWays->size(); i++)
                    if (analysedWays->at(i)->numOfCrosslines >= firstWay->numOfCrosslines)
                    {
                        delete analysedWays->at(i);
                        analysedWays->remove(i);
                    }

            }

            textBrowserMenu->append("Optimal way is:");
            firstWay->printWay(textBrowserMenu);
            delete firstWay;
            for (int i = 0; i < analysedWays->size(); i++)
                delete analysedWays->at(i);
            for (int i = 0; i < newWays->size(); i++)
                delete newWays->at(i);
        }
    }

    //GUI - OK
    void findTheWay(Station *stationFrom, Station *stationTo, QTextBrowser *textBrowserMenu)
    {
        Way *way = new Way();
        way->stations.append(stationFrom);
        way->numOfCrosslines = 0;
        way->totalweight = 0;
        QVector<Way*> *analysedWays = new QVector<Way*>;
        QVector<Way*>  *newWays = new QVector<Way*>;
        analysedWays->append(way);

        bool someWayIsFound = false;
        bool noWaysMore = false;

        Way *firstWay = new Way();
        firstWay->totalweight = INT_MAX;
        while (!someWayIsFound && !noWaysMore)
        {

            /* cout << "Start of circle: newWays: ";  //
           for (int i = 0; i < newWays->size(); i++)
           newWays->at(i)->printWay();
           cout << "analysed: ";
           for (int i = 0; i < analysedWays->size(); i++)
           analysedWays->at(i)->printWay();      */           //

            noWaysMore = true;
            for (int indWay = 0; indWay < analysedWays->size(); indWay++)
            {
                for (int indTransfer = 0; indTransfer < analysedWays->at(indWay)->stations.last()->transfers.size(); indTransfer++)
                {
                    if (!analysedWays->at(indWay)->stationContains(analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to))
                    {
                        Way *newWay = new Way(*analysedWays->at(indWay));

                        noWaysMore = false;

                        if (analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to->line != analysedWays->at(indWay)->stations.last()->line)
                            newWay->numOfCrosslines += 1;

                        newWay->totalweight += analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->weight;

                        newWay->stations.append(analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to);

                        if (analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to == stationTo && newWay->totalweight < firstWay->totalweight)
                        {
                            someWayIsFound = true;
                            firstWay = newWay;
                        }
                        else
                            newWays->append(newWay);
                    }
                }
            }
            //           cout << "BEFORE CHOOSING firstWay - Alternative unfinished ways (newWays) " << newWays->size() << "\n";
            //           for (int i = 0; i < newWays->size(); i++)
            //           newWays->at(i)->printWay();

            analysedWays = new QVector<Way*>(*newWays);
            newWays->clear();

            //           cout << "BEFORE CHOOSING firstWay - Alternative unfinished ways (analysed) " << analysedWays->size() << "\n";
            //           for (int i = 0; i < analysedWays->size(); i++)
            //           analysedWays->at(i)->printWay();
        }

        if (!someWayIsFound)
            textBrowserMenu->append("There is no way between the stations\n");
        else
        {
            //           cout << "Firstly found way:\n";
            //           firstWay->printWay();
            //           cout << "AFTER CHOOSING firstWay - Alternative unfinished ways\n";
            for (int i = 0; i < analysedWays->size(); i++)
            {
                //               analysedWays->at(i)->printWay();
                if (analysedWays->at(i)->totalweight >= firstWay->totalweight)
                {
                    delete analysedWays->at(i);
                    analysedWays->remove(i);
                }
            }
//            for (int i = 0; i < analysedWays->size(); i++)
//                analysedWays->at(i)->printWay(textBrowserMenu);

            while (analysedWays->size() > 0 && !noWaysMore)
            {
                noWaysMore = true;
                for (int indWay = 0; indWay < analysedWays->size(); indWay++)
                {
                    for (int indTransfer = 0; indTransfer < analysedWays->at(indWay)->stations.last()->transfers.size(); indTransfer++)
                    {
                        if (!analysedWays->at(indWay)->stationContains(analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to))
                        {
                            Way *newWay = new Way(*analysedWays->at(indWay));
                            noWaysMore = false;

                            if (analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to->line != analysedWays->at(indWay)->stations.last()->line)
                                newWay->numOfCrosslines += 1;

                            newWay->totalweight += analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->weight;

                            newWay->stations.append(analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to);

                            if (analysedWays->at(indWay)->stations.last()->transfers.at(indTransfer)->to == stationTo)
                            {
                                if (newWay->totalweight < firstWay->totalweight)
                                    firstWay = newWay;
                            }
                            else
                            {
                                newWays->append(newWay);
                            }
                        }
                    }
                }

                for (int i = 0; i < analysedWays->size(); i++)
                    delete analysedWays->at(i);

                delete analysedWays;
                analysedWays = new QVector<Way*>(*newWays);
                newWays->clear();


                for (int i = 0; i < analysedWays->size(); i++)
                    if (analysedWays->at(i)->totalweight >= firstWay->totalweight)
                    {
                        delete analysedWays->at(i);
                        analysedWays->remove(i);
                    }

            }

            textBrowserMenu->append("The fastest way is:");
            firstWay->printWay(textBrowserMenu);
            delete firstWay;
            for (int i = 0; i < analysedWays->size(); i++)
                delete analysedWays->at(i);
            for (int i = 0; i < newWays->size(); i++)
                delete newWays->at(i);
        }

    }

    string nameOfMap;
    QMap<string, Station*> stations;
    int numberOfLines;
};
