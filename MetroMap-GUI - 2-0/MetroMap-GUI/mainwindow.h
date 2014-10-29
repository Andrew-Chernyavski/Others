#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "transfer.h"
#include "Station.h"
#include "Map.h"
#include "QFile"

#pragma once

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void printLine (Map* openedMap, int line);
    void printMap (Map* openedMap);
    void hideMapMenu();
    ~MainWindow();

private slots:
    void slotButton1();
    void slotButton2();
    void slotButtonInput();
    void slotButtonExit();

    void slotAddLine();
    void slotDeleteLine();
    void slotAddStation();
    void slotDeleteStation();
    void slotRenameStation();
    void slotAddTransfer();
    void slotDeleteTransfer();
    void slotEditTransfer();
    void slotFindTheWayFastest();
    void slotFindTheWayMinCross();
    void slotFindTheWayThroughStation();
    void slotExportInXML();
    void slotCancel();

private:
    QList<Map*> *uploadedMaps;
    void uploadedMapsMode();
    void mapIsOpenedMode();
    void createMapMode();

    Map* openedMap;

    Station *newStation;
    Station *prevStation;
    Station *helpStation;
    Transfer *newTransfer;
    Transfer *prevTransfer;

    Ui::MainWindow *ui;

    int state;
    int numOfAddingLinesLeft;
    int numOfAddingStationsLeft;
    int numOfAddingTransfersLeft;
};

#endif // MAINWINDOW_H
