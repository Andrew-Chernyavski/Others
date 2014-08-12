#ifndef TRANSFER_H
#define TRANSFER_H

#pragma once

class Station;

class Transfer
{
public:
    int weight;
    Station *to;
};

#endif // TRANSFER_H
