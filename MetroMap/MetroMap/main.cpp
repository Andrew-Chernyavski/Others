#include <QCoreApplication>
#include <iostream>
#include <QtCore/QVector>
#include <QMap>
#include <QList>

#define noMaps 0
#define mapsAreUploaded 1
#define mapsIsOpened 2
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
    string nameOfStation;
    int id;
    int line;
    QVector<Transfer*> transfers;
};

class Map
{
public:
    void fillMap()
    {
        int idOfStation = 0;
        cout << "Input the name of new map: ";
        cin >> this->nameOfMap;
        cout << "The mask of line is: STATION + [WEIGHT + STATION]\n";
        cout << "Transfers will be added later\n";
        cout << "\nInput the number of lines you are going to add: ";
        cin >> this->numberOfLines;
        for (int i = 0; i < this->numberOfLines;)
        {
            cout << "Filling line " << ++i << ": input the number of stations: ";
            int numOfStations = 0;
            cin >> numOfStations;

            //Первая станция запихивается без ссылок на соседние станции, поэтому вне цикла
            Station *newStation = new Station();
            cout << "Input the name of first station: ";
            cin >> newStation->nameOfStation;
            newStation->id = ++idOfStation;
            newStation->line = i;
            this->stations.insert(newStation->nameOfStation, newStation);

            Station *prevStation = newStation;
            for (int j = 1; j < numOfStations; j++)
            {
                newStation = new Station();
                newStation->id = ++idOfStation;
                newStation->line = i;
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
                prevStation = newStation;
            }
            cout << "Filling line " << i << " finished\n\n";
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
void printMap (Map* openedMap)
{
    cout << "Opened map: " << openedMap->nameOfMap << "\n";
    for (int i = 1; i <= openedMap->numberOfLines; i++)
    {
        cout << "  Stations of line " << i << ":\n";
        QMap<string, Station*>::iterator ind;
         for (ind = openedMap->stations.begin(); ind != openedMap->stations.end(); ind++)
             if (ind.value()->line == i)
             {
                 cout << "    " << ind.value()->nameOfStation << "\n";
                 cout << "      Transfers: ";
                 for (int indTransf = 0; indTransf < ind.value()->transfers.length(); indTransf++)
                 {
                     cout << ind.value()->transfers[indTransf]->to->nameOfStation << " (";
                     if (ind.value()->transfers[indTransf]->to->line == i)
                         cout << "Neighbour, ";
                     else
                         cout << "Crossline to " << ind.value()->transfers[indTransf]->to->line << ", ";
                     cout << ind.value()->transfers[indTransf]->weight << "min) . ";
                 }
                 cout << "\n";
             }
         //cout << "\n";
    }
    cout << "\n";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<Map*> *uploadedMaps = new QList<Map*>();
    Map* openedMap = NULL;
    int command = 0;
    int state = mapsAreUploaded;
    if (uploadedMaps->isEmpty())
        state = noMaps;

    while (state != finish)
    {
        switch (state) {
        case noMaps:
            cout << "You don't have any maps\n * Press 1 to create a new map\n * Press 2 to exit\n";
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
            //cout << " - Input "
            state = finish;
        }
    }


    return 0;
}
