#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <iostream>
#include <sstream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>

using namespace std;

struct rectangle
{
    int x;
    int y;
    int width;
    int height;
};

int getMax (int mas[], int n)
{
    //int num = 1;
    int max = mas[0];
    for (int i = 1; i < n; i++)
    {
        if (mas[i] > max)
        {
            max = mas[i];
            //num = 1;
        }
        //        else if (mas[i] == max)
        //            num++;
    }
    return max;// + 0.01*num;
}

int getIndMin (int mas[], int n)
{
    int min = INT_MAX;
    int ind = 0;
    for (int i = 0; i < n; i++)
        if ((mas[i] < min) && (mas[i] > 0))
        {
            ind = i;
            min = mas[i];
        }
    return ind;
}

void unite(QVector<rectangle> &Recs, int i, int j)
{
    if ((Recs[i].x == Recs[j].x) && (Recs[i].width == Recs[j].width))//прямоугольники на одной вертикальной полосе
    {
        //либо такая ситуация [ { ] }
        if ((Recs[i].y <= Recs[j].y) && (Recs[i].y + Recs[i].height >= Recs[j].y))
        {
            rectangle rec;
            rec.x = Recs[i].x;
            rec.width = Recs[i].width;
            rec.y = Recs[i].y;
            rec.height = max(Recs[j].y + Recs[j].height, Recs[i].y + Recs[i].height) - Recs[i].y;
            Recs.remove(i);
            Recs.remove(j);
            Recs.append(rec);
            cout << "United: " << i << " " << j << "\n";
        }

        //либо наоборот { [ } ]
        else if ((Recs[j].y <= Recs[i].y) && (Recs[j].y + Recs[j].height >= Recs[i].y))
        {
            rectangle rec;
            rec.x = Recs[i].x;
            rec.width = Recs[i].width;
            rec.y = Recs[j].y;
            rec.height = max(Recs[j].y + Recs[j].height, Recs[i].y + Recs[i].height) - Recs[j].y;
            Recs.remove(i);
            Recs.remove(j);
            Recs.append(rec);
            cout << "United: " << i << " " << j << "\n";
        }

        else
            cout << "Cannot be unite! vertical " << i << " " << j << "\n";
    }
    else if ((Recs[i].y == Recs[j].y) && (Recs[i].height == Recs[j].height))//одна горизонтальная полоса
    {
        if ((Recs[i].x <= Recs[j].x) && (Recs[i].x + Recs[i].width >= Recs[j].x))
        {
            rectangle rec;
            rec.y = Recs[i].y;
            rec.height = Recs[i].height;
            rec.x = Recs[i].x;
            rec.width = max(Recs[j].x + Recs[j].width, Recs[i].x + Recs[i].width) - Recs[i].x;
            Recs.remove(i);
            Recs.remove(j);
            Recs.append(rec);
            cout << "United: " << i << " " << j << "\n";//на случай ошибочного попадания
        }

        else if ((Recs[j].x <= Recs[i].x) && (Recs[j].x + Recs[j].width >= Recs[i].x))
        {
            rectangle rec;
            rec.y = Recs[i].y;
            rec.height = Recs[j].height;
            rec.x = Recs[j].x;
            rec.width = max(Recs[j].x + Recs[j].width, Recs[i].x + Recs[i].width) - Recs[j].x;
            Recs.remove(i);
            Recs.remove(j);
            Recs.append(rec);
            cout << "United: " << i << " " << j << "\n";
        }

        else
            cout << "Cannot be unite! horizontal " << i << " " << j << "\n";//на случай ошибочного попадания
    }
    else
        cout << "Error! Cannot be unite! non vertical or horizontal " << i << " " << j << "\n" << "i, j: y: " << Recs[i].y << " " << Recs[j].y <<"; height: " << Recs[i].height << " " << Recs[j].height << "\n";//на случай ошибочного попадания
}

int main(int argc, char *argv[])
{
    //входные данные - текстовый файл
    //каждая строка файла - информация о прямоугольнике, хранящая 4 значения int
    //x,y - координаты левого верхнего угла
    //width, height - ширина и высота прямоугольника
    //защиты от дурака нет - предполагается, что входные данные оформлены верно
    QApplication app(argc, argv);
    QGraphicsScene scene;
    scene.setSceneRect(-300, -300, 600, 600);
    QGraphicsView view(&scene);

    QVector<rectangle> Recs;
    QFile textFile("input.txt");
    if(!textFile.open(QIODevice::ReadOnly)) {
        scene.addText("Cannot open input file");
    }
    else
    {
        QTextStream in(&textFile);
        while(!in.atEnd()) {
            QString line = in.readLine();
            string str = line.toStdString();
            stringstream stream(str);
            rectangle rec;
            stream >> rec.x;
            stream >> rec.y; rec.y -= 250;//смещение в верхнюю часть для более удобного отображения
            stream >> rec.width;
            stream >> rec.height;
            QGraphicsRectItem *item = new QGraphicsRectItem(rec.x, rec.y, rec.width, rec.height);
            item -> setBrush(QBrush(Qt::white));
            scene.addItem(item);
            Recs.push_back(rec);
        }
        //считали все данные из файла
        view.show();


        double changed = false;//индикатор перестроения системы
        //если за весь цикл перестроения не произошло - мы пришли к окончательному варианту

        int counter = 1; //для пошагового отображения
        while (!changed)
        {
            changed = false;
            int n = Recs.size();
            int maxSize[n][n];//таблица объединяемых прямоугольников
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    maxSize[i][j] = 0;//значение 0 означает что прямоугольники i и j не являются кандидатами на объединение

            for (int i = 0; i < n - 1; i++)//набираем кандидатов на объединение
            {                              //сделано для возможного расширения системы
                for (int j = i + 1; j < n; j++)//сравнение всех возможных прямоугольников между собой
                {
                    if ((Recs[i].x == Recs[j].x) && (Recs[i].width == Recs[j].width))//прямоугольники на одной вертикальной полосе
                    {
                        //либо оба прямоугольника соприкоснулись началом
                        if (Recs[i].y == Recs[j].y) {
                            maxSize[i][j] = max(max(Recs[i].height, Recs[j].height),Recs[i].width);
                            maxSize[j][i] = maxSize[i][j];
                        }

                        //либо такая ситуация [ { ] }
                        else if ((Recs[i].y < Recs[j].y) && (Recs[i].y + Recs[i].height >= Recs[j].y))
                        {
                            maxSize[i][j] = max(Recs[i].height, Recs[j].y + Recs[j].height - Recs[i].y);
                            maxSize[j][i] = maxSize[i][j];
                        }

                        //либо наоборот { [ } ]
                        else if ((Recs[j].y < Recs[i].y) && (Recs[j].y + Recs[j].height >= Recs[i].y))
                        {
                            maxSize[i][j] = max(Recs[i].height, Recs[j].y + Recs[j].height - Recs[i].y);
                            maxSize[j][i] = maxSize[i][j];
                        }

                        else
                            changed = false;
                    }
                    else if ((Recs[i].y == Recs[j].y) && (Recs[i].height == Recs[j].height))//одна горизонтальная полоса
                    {
                        if (Recs[i].x == Recs[j].x)
                        {
                            maxSize[i][j] = max(max(Recs[i].width, Recs[j].width),Recs[i].height);
                            maxSize[j][i] = maxSize[i][j];
                        }

                        else if ((Recs[i].x < Recs[j].x) && (Recs[i].x + Recs[i].width >= Recs[j].x))
                        {
                            maxSize[i][j] = max(Recs[i].width, Recs[j].x + Recs[j].width - Recs[i].x);
                            maxSize[j][i] = maxSize[i][j];
                        }

                        else if ((Recs[j].x < Recs[i].x) && (Recs[j].x + Recs[j].width >= Recs[i].x))
                        {
                            maxSize[i][j] = max(Recs[i].width, Recs[i].x + Recs[i].width - Recs[j].x);
                            maxSize[j][i] = maxSize[i][j];
                        }

                        else
                            changed = false;
                    }
                    else
                        cout << "Rectangles " << i << ", " << j << ": no any unite\n";
                }
            }

            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                    cout << maxSize[i][j] << " ";
                cout << "\n";
            }
            cout << "\n";

            //если изменений не произошло - мы достигли оптимальной конструкции
            bool nullable = true;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (maxSize[i][j] > 0)
                        nullable = false;
            if (nullable)
            {
                break;
                cout << "The End\n";
                changed = false;
            }
                        else
                        {
                            QVector<rectangle> Recs2(Recs);

                            bool flag = true;
                            int i = 0;
                            while (flag && (i < n))
                            {
                                if (getMax(maxSize[i],n) > 0)
                                {
                                    int indMin = getIndMin(maxSize[i], n);//пришли к тому, что будем объединять прямоугольники с индексами i и indMin
                                    cout << "we should unite " << indMin << " and " << i << "\n";
                                    unite(Recs2,indMin,i);
                                    for (int k = 0; k < n; k++)
                                    {
                                        maxSize[i][k] = 0;
                                        maxSize[k][i] = 0;
                                        maxSize[indMin][k] = 0;
                                        maxSize[k][indMin] = 0;
                                    }
                                    flag = false;
                                }
                                i++;
                            }
                            cout << "Size of Recs2: " << Recs2.size() << "\n";
                            Recs = Recs2;
                        }

            for (int i = 0; i < Recs.size(); i++)
            {
                QGraphicsRectItem *item = new QGraphicsRectItem(Recs[i].x, Recs[i].y + counter*80, Recs[i].width, Recs[i].height);
                item -> setBrush(QBrush(Qt::white));
                scene.addItem(item);
            }
            view.show();
            counter++;
            system("pause");
        }//and of while


    }
    textFile.close();
    return app.exec();
}
