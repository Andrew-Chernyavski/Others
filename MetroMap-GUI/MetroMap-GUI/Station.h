#include <QVector>
#include "transfer.h"
#include <iostream>

#pragma once

using namespace std;

class Station
{
public:
    bool stationsIsDealEnd ()
    {
        int neighbours = 0;
        for (int i = 0; i < this->transfers.size(); i++)
            if (this->transfers[i]->to->line == this->line)
                neighbours++;
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
