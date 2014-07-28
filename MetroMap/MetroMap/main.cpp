#include <QCoreApplication>
#include <iostream>
#include <QtCore/QVector>
#include <QMap>
#include <QList>

#define noMaps 0
#define mapsAreUploaded 1
#define mapsIsOpened 2
#define findingTheWay 3
#define editingMapMode 4
#define editingLineMode 5

#define finish 14

using namespace std;

class Transfer;
class Station;

class Transfer
{
public:
    int weight;
    Station *to;
};

class Station
{
public:
    bool stationsIsDealEnd ()
    {
        int neighbours = 0;
        for (int i = 0; i < this->transfers.size(); i++)
            if (this->transfers[i]->to->line == this->line)
                neighbours++;
        if (neighbours > 2 || neighbours == 0)
            cout << "ERROR! Station " << this->nameOfStation << " has " << neighbours << " neighbours!\n";
        if (neighbours == 1)
            return true;
        else
            return false;
    }
    bool isNeighbour (Station *station1)
    {
        if (station1->line != this->line)
            return false;
        for (int i = 0; i < station1->transfers.size(); i++)
            if (station1->transfers.at(i)->to == this)
                return true;
        return false;
    }

    bool isConnectedWith (Station *station1)
    {
        for (int i = 0; i < station1->transfers.size(); i++)
            if (station1->transfers.at(i)->to == this)
                return true;
        return false;
    }

    Transfer *getConnection (Station *station1)
    {
        for (int i = 0; i < station1->transfers.size(); i++)
            if (station1->transfers.at(i)->to == this)
                return station1->transfers.at(i);
        return NULL;
    }

    string nameOfStation;
    int line;
    QVector<Transfer*> transfers;
};

class Way
{
public:
  QVector<Station*> stations;
  int totalweight;
  int numOfCrosslines;

  bool stationContains (Station *station)
  {
      for (int i = 0; i < this->stations.size(); i++)
          if (this->stations.at(i) == station)
              return true;
      return false;
  }

  void printWay()
  {
      for (int i = 0; i < this->stations.size(); i++)
          cout << this->stations.at(i)->nameOfStation << "; ";
      cout << "\nTime: " << this->totalweight << "; Crosslines: " << this->numOfCrosslines << "\n";
  }
};

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

    void findTheWayLessCrosslines (Station *stationFrom, Station *stationTo)
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
            cout << "There is no way between the stations\n";
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

            cout << "Optimal way is:\n";
            firstWay->printWay();
            delete firstWay;
            for (int i = 0; i < analysedWays->size(); i++)
                delete analysedWays->at(i);
            for (int i = 0; i < newWays->size(); i++)
                delete newWays->at(i);
        }
    }

    void findTheWay(Station *stationFrom, Station *stationTo)
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
           cout << "There is no way between the stations\n";
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
           for (int i = 0; i < analysedWays->size(); i++)
               analysedWays->at(i)->printWay();

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

           cout << "Optimal way is:\n";
           firstWay->printWay();
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

void createMap(QList<Map*> *uploadedMaps)
{
    Map *newMap = new Map();
    uploadedMaps->append(newMap);
    newMap->fillMap();
}


//Если сможем гарантировать, что внутри Map
//список станций будет храниться по линиям
//то можно оптимизировать в один проход
void printLine (Map* openedMap, int line)
{
    cout << "  Stations of line " << line << ":\n";
    QMap<string, Station*>::iterator ind;
     for (ind = openedMap->stations.begin(); ind != openedMap->stations.end(); ind++)
         if (ind.value()->line == line)
         {
             cout << "    " << ind.value()->nameOfStation << "\n";
             cout << "      Transfers: ";
             for (int indTransf = 0; indTransf < ind.value()->transfers.length(); indTransf++)
             {
                 cout << ind.value()->transfers[indTransf]->to->nameOfStation << " (";
                 if (ind.value()->transfers[indTransf]->to->line == line)
                     cout << "Neighbour, ";
                 else
                     cout << "Crossline to " << ind.value()->transfers[indTransf]->to->line << ", ";
                 cout << ind.value()->transfers[indTransf]->weight << "min) . ";
             }
             cout << "\n";
         }
}

void printMap (Map* openedMap)
{
    cout << "\nOpened map: " << openedMap->nameOfMap << "\n";
    for (int i = 1; i <= openedMap->numberOfLines; i++)
        printLine(openedMap, i);
    cout << "\n";
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<Map*> *uploadedMaps = new QList<Map*>();

    //TODO: загрузка карт в формате json

    Map* openedMap = NULL;
    int command = 0;
    int state = mapsAreUploaded;
    if (uploadedMaps->isEmpty())
        state = noMaps;

    Station *stationFrom;
    Station *stationTo;
    Station *station1;
    string nameOfStationFrom;
    string nameOfStationTo;//для прокладки машрутов
    string nameOfStation1;

    while (state != finish)
    {
        switch (state) {
        case noMaps:
            cout << "You don't have any maps\n * Input 1 to create a new map\n * Input 2 to exit\n";
            cin >> command;
            switch (command) {
            case 1:
                createMap(uploadedMaps);
                state = mapsAreUploaded;
                break;
            case 2:
                state = finish;
                break;
            default:
                cout << "What?\n";
                break;
            }

            break;

        case mapsAreUploaded:
            cout << "Available maps:\n";
            for (int i = 0; i < uploadedMaps->length(); i++)
                cout << " - " << i + 1 << ": " << uploadedMaps->at(i)->nameOfMap << "\n";
            cout << " * Input the number of the map to open it\n";
            cout << " * Input 0 to exit\n * Input any other number to create a new map\n";
            cin >> command;
            if (command == 0)
                state = finish;
            else if (command > 0 && command <= uploadedMaps->length())
            {
                openedMap = uploadedMaps->at(command - 1);
                state = mapsIsOpened;
            }
            else
                createMap(uploadedMaps);
            break;

        case mapsIsOpened:
            printMap(openedMap);
            cout << "\n";
            command = 0;
            cout << " * Input 1 to find the way between the stations\n";
            cout << " * Input 2 to edit the elements of the map\n";
            cout << " * Input 3 to save this map in json\n";
            cout << " * Input 4 to close this map and go to the main menu\n";
            cout << " * Input 0 to exit\n";
            cin >> command;
            switch (command) {
            case 1:
                state = findingTheWay;
                break;
            case 2:
                state = editingMapMode;
                break;
            case 3:
                //TODO: запись текущей карты в json
                break;
            case 4:
                state = mapsAreUploaded;
                break;
            case 0:
                state = finish;
                break;
            default:
                cout << "Wrong command!\n";
                break;
            }
            break;

        case findingTheWay:
            //TODO: прокладка маршрутов с опциями и блекджеком

            cout << "Input the stations FROM and TO\n";
            cin >> nameOfStationFrom;
            if (!openedMap->stations.contains(nameOfStationFrom))
            {
                cout << "Error: the first station does not exist\n";
                break;
            }
            stationFrom = openedMap->stations.value(nameOfStationFrom);
            cin >> nameOfStationTo;
            if (!openedMap->stations.contains(nameOfStationTo))
            {
                cout << "Error: the second station does not exist\n";
                break;
            }
            stationTo = openedMap->stations.value(nameOfStationTo);

            cout << " * Input 1 to find the fastest way\n";
            cout << " * Input 2 to find the way with the least number of crossline transfers\n";
            cout << " * Input 3 to visit another station\n";
            cout << " * Input something else to cancel\n";
            cin >> command;
            switch (command) {
            case 1:
                openedMap->findTheWay(stationFrom, stationTo);
                break;
            case 2:
                openedMap->findTheWayLessCrosslines(stationTo, stationFrom);
                break;
            case 3:
                cout << "Input the name of another station\n";
                cin >> nameOfStation1;
                if (!openedMap->stations.contains(nameOfStation1))
                {
                    cout << "Error: these station does not exist\n";
                    break;
                }
                station1 = openedMap->stations.value(nameOfStation1);
                cout << "That's you pair of ways:\n";
                openedMap->findTheWay(stationFrom, station1);
                openedMap->findTheWay(station1, stationTo);
                break;
            default:
                break;
            }



            state = mapsIsOpened;
            break;

        case editingMapMode:
            cout << " * Input 1 to add a new line (without crosslines)\n";
            cout << " * Input 2 to edit an existing line\n";
            cout << " * Input 3 to close the map editing mode\n";
            command = 0;
            cin >> command;
            switch (command) {
            case 3:
                state = mapsIsOpened;
                break;
            case 1:
                openedMap->addingLine(openedMap->numberOfLines);
                openedMap->numberOfLines++;
                break;
            case 2:
                state = editingLineMode;
                break;
            default:
                cout << "Wrong command!\n";
                break;
            }
            break;

        case editingLineMode:
            cout << " * Input 1 to create a new station\n";
            cout << " * Input 2 to edit or remove an existing station\n";
            cout << " * Input 3 to create a new crossline transfer\n";
            cout << " * Input 4 to edit or remove an existing transfer\n";
            cout << " * Input 5 to close the line editing mode\n";
            command = 0;
            int numOfLine = 0;
            QMap<string, Station*>::iterator ind;
            QList<Station*> *dealEnds = new QList<Station*>;
            Station *endStation; //нужно для режима добавления новой конечной станции
            Station *station1;//режимы вставки станций, удаление из промежностей
            Station *station2;//режимы вставки станций, удаление из промежностей
            Station *station3; //удаление станции из промежностей
            bool findFirstNeighbour;//удаление станции из промежностей
            string nameOfStation1;//режимы insert станции
            string nameOfStation2;//режим insert станции
            cin >> command;
            switch (command) {
            case 1:

                cout << "Input the number of existing line to add station\n";
                cin >> numOfLine;
                printLine(openedMap, numOfLine);
                cout << " * Input 1 to add new deal-end station\n";
                cout << " * Input 2 to insert new station between pair of existing\n";
                cout << " * Input 3 to cancel\n";
                cin >> command;
                switch (command) {
                case 1:
                    for (ind = openedMap->stations.begin(); ind != openedMap->stations.end(); ind++)
                        if (ind.value()->line == numOfLine && ind.value()->stationsIsDealEnd())
                            dealEnds->append(ind.value());
                    for (int i = 0; i < dealEnds->size(); i++)
                        cout << " * Input " << i + 1 << " to add new station after '" << dealEnds->at(i)->nameOfStation << "'\n";
                    cin >> command;
                    endStation = openedMap->addStationAfter(dealEnds->at(command - 1));
                    break;
                case 2:
                    cout << "Input the stations between of which you're going to insert new station\n";
                    cin >> nameOfStation1;
                    if (!openedMap->stations.contains(nameOfStation1))
                    {
                        cout << "Error: the first station does not exist\n";
                        break;
                    }
                    station1 = openedMap->stations.value(nameOfStation1);
                    cin >> nameOfStation2;
                    if (!openedMap->stations.contains(nameOfStation2))
                    {
                        cout << "Error: the second station does not exist\n";
                        break;
                    }
                    station2 = openedMap->stations.value(nameOfStation2);
                    if (!station1->isNeighbour(station2))
                    {
                        cout << "Error: stations are not neighbours!\n";
                        break;
                    }
                    else
                    {
                        Station *newStation = new Station();
                        cout << "Input the name of new station\n";
                        cin >> newStation->nameOfStation;
                        newStation->line = station1->line;
                        Transfer *transfer;
                        for (int i = 0; i < station1->transfers.size(); i++)
                            if (station1->transfers.at(i)->to == station2)
                            {

                                cout << "Input the time between stations " << newStation->nameOfStation << " and ";
                                cout << station1->nameOfStation << ": ";
                                cin >> station1->transfers.at(i)->weight;
                                station1->transfers.at(i)->to = newStation;
                                transfer = new Transfer();
                                transfer->weight = station1->transfers.at(i)->weight;
                                transfer->to = station1;
                                newStation->transfers.append(transfer);
                            }

                        for (int i = 0; i < station2->transfers.size(); i++)
                            if (station2->transfers.at(i)->to == station1)
                            {
                                cout << "Input the time between stations " << newStation->nameOfStation << " and ";
                                cout << station2->nameOfStation << ": ";
                                cin >> station2->transfers.at(i)->weight;
                                station2->transfers.at(i)->to = newStation;
                                transfer = new Transfer();
                                transfer->weight = station2->transfers.at(i)->weight;
                                transfer->to = station2;
                                newStation->transfers.append(transfer);
                            }
                        openedMap->stations.insert(newStation->nameOfStation, newStation);
                        cout << "Station " << newStation->nameOfStation << " succesfully added\n";
                    }
                    break;
                case 3:
                    break;
                default:
                    cout << "Wrong command!\n";
                    break;
                }

                break;
            case 2:
                cout << "Input the station you're going to remove or edit\n";
                cin >> nameOfStation1;
                if (!openedMap->stations.contains(nameOfStation1))
                {
                    cout << "Error: this station does not exist\n";
                    break;
                }
                station1 = openedMap->stations.value(nameOfStation1);
                cout << " * Input 1 to rename the station\n";
                cout << " * Input 2 to remove the station\n";
                cout << " * Input 3 to cancel\n";
                cin >> command;
                switch (command) {
                case 1:
                    cout << "Input new name of station";
                    cin >> nameOfStation1;
                    if (openedMap->stations.contains(nameOfStation1))
                    {
                        cout << "This name is already used!\n";
                        break;
                    }
                    else
                        station1->nameOfStation = nameOfStation1;
                    break;
                case 2:
                    for (ind = openedMap->stations.begin(); ind != openedMap->stations.end(); ind++)
                        for (int i = 0; i < ind.value()->transfers.size(); i++)
                            if (ind.value()->transfers.at(i)->to == station1)
                                ind.value()->transfers.remove(i);//неэффективный способ стереть все переходы на станцию

                    if (station1->stationsIsDealEnd())
                    {
                        for (int i = 0; i < station1->transfers.size(); i++)

                        station1->transfers.clear();
                        openedMap->stations.remove(openedMap->stations.key(station1));
                    }
                    else
                    {
                        findFirstNeighbour = false;
                        for (int i = 0; i < station1->transfers.size(); i++)
                        {
                            if (station1->transfers.at(i)->to->line == station1->line)
                            {
                                if (!findFirstNeighbour)
                                {
                                    station2 = station1->transfers.at(i)->to;
                                    findFirstNeighbour = true;
                                }
                                else
                                    station3 =station1->transfers.at(i)->to;
                            }
                        }
                        //это мы так в station2,3 записали соседей

                        station1->transfers.clear();
                        openedMap->stations.remove(openedMap->stations.key(station1));
                        //собственно сносим станцию

                        cout << "Input new time taken between " << station2->nameOfStation;
                        cout << " and " << station3->nameOfStation << "\n";

                        Transfer *transfer1 = new Transfer();
                        Transfer *transfer2 = new Transfer();
                        cin >> transfer1->weight;
                        transfer2->weight = transfer1->weight;
                        transfer1->to = station2;
                        station3->transfers.append(transfer1);
                        transfer2->to = station3;
                        station2->transfers.append(transfer2);

                    }
                    break;
                case 3:
                    break;
                default:
                    cout << "Wrong command!\n";
                    break;
                }

                break;
            case 3:
                cout << "Input the stations you're going to connect\n";
                cin >> nameOfStation1;
                if (!openedMap->stations.contains(nameOfStation1))
                {
                    cout << "Error: the first station does not exist\n";
                    break;
                }
                station1 = openedMap->stations.value(nameOfStation1);
                cin >> nameOfStation2;
                if (!openedMap->stations.contains(nameOfStation2))
                {
                    cout << "Error: the second station does not exist\n";
                    break;
                }
                station2 = openedMap->stations.value(nameOfStation2);
                if (station1->line == station2->line)
                {
                    cout << "Error: these stations are from the same line\n";
                    break;
                }
                else
                {
                    Transfer *transfer1 = new Transfer();
                    Transfer *transfer2 = new Transfer();
                    cout << "Input the time taken to get between the stations\n";
                    cin >> transfer1->weight;
                    transfer2->weight = transfer1->weight;
                    transfer1->to = station2;
                    transfer2->to = station1;
                    station1->transfers.append(transfer1);
                    station2->transfers.append(transfer2);

                }
                break;
            case 4:
                cout << " * Input 1 to change the weight of transfer\n";
                cout << " * Input 2  to remove crossline transfer\n";
                cout << " * Input something else to cancel\n";
                cin >> command;
                switch (command) {
                case 1:
                    cout << "Input the name of stations, which transfer you want to change\n";
                    cin >> nameOfStation1;
                    if (!openedMap->stations.contains(nameOfStation1))
                    {
                        cout << "Error: the first station does not exist\n";
                        break;
                    }
                    station1 = openedMap->stations.value(nameOfStation1);
                    cin >> nameOfStation2;
                    if (!openedMap->stations.contains(nameOfStation2))
                    {
                        cout << "Error: the second station does not exist\n";
                        break;
                    }
                    station2 = openedMap->stations.value(nameOfStation2);
                    if (!station1->isConnectedWith(station2))
                    {
                        cout << "Error: stations hasn't anu connections\n";
                        break;
                    }
                    else
                    {
                        cout<< "Current weight is " << station1->getConnection(station2)->weight;
                        cout << "\n * Input new weight\n";
                        cin >> station1->getConnection(station2)->weight;
                        station2->getConnection(station1)->weight = station1->getConnection(station2)->weight;
                    }
                    break;
                case 2:
                    cout << "Input the name of stations, which transfer you want to change\n";
                    cin >> nameOfStation1;
                    if (!openedMap->stations.contains(nameOfStation1))
                    {
                        cout << "Error: the first station does not exist\n";
                        break;
                    }
                    station1 = openedMap->stations.value(nameOfStation1);
                    cin >> nameOfStation2;
                    if (!openedMap->stations.contains(nameOfStation2))
                    {
                        cout << "Error: the second station does not exist\n";
                        break;
                    }
                    station2 = openedMap->stations.value(nameOfStation2);
                    if (!station1->isConnectedWith(station2) || station1->isNeighbour(station2))
                    {
                        cout << "It it not crossline transfer\n";
                    }
                    else
                    {
                        for (int i = 0; i < station1->transfers.size(); i++)
                            if (station1->transfers.at(i)->to == station2)
                                station1->transfers.remove(i);

                        for (int i = 0; i < station2->transfers.size(); i++)
                            if (station2->transfers.at(i)->to == station1)
                                station2->transfers.remove(i);

                    }
                    break;
                default:
                    break;
                }

            case 5:
                state = editingMapMode;
                break;
            default:
                cout << "Wrong command!\n";
                break;
            }
        }
    }

    cout << "Input something to close me...\n";
    return 0;
}
