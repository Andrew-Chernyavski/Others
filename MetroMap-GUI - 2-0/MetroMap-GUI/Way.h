#include "Station.h"
#include "transfer.h"

#pragma once
#include <QTextBrowser>

using namespace std;

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

    //OK
    void printWay(QTextBrowser *textBrowserMenu)
    {
        QString str = "";
        for (int i = 0; i < this->stations.size(); i++)
            str += QString::fromStdString(this->stations.at(i)->nameOfStation + "; ");
        textBrowserMenu->append(str);

        textBrowserMenu->append("Time: " + QString::number(this->totalweight) + "; Crosslines: " + QString::number(this->numOfCrosslines) + "\n");
    }
};
