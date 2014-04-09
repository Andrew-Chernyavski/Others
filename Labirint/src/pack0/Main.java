package pack0;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Andrew on 08.04.14.
 */
public class Main {
    final static int height = 10;
    final static int width = 10;
    //Для простоты считаем, что перегородки в лабиринте занимают не пространства между клетками, а сами клетки

    public static void printMat (int[][] a) {
        for (int i = 0; i < a.length; i++) {
            for (int j = 0; j < a[i].length; j++)
                System.out.print(a[i][j] + " ");
            System.out.println();
        }
    }

    static int findWay(int steps, List<Pair> Pairs, int[][] Lab, Pair finish) {
        if (Pairs.isEmpty()) {
            System.out.println("Добраться до точки финиша невозможно");
            return -1;
        }
        List<Pair> Outp = new ArrayList<Pair>();
        for (Pair pair : Pairs) {
            int x = pair.getX();
            int y = pair.getY();
            List<Pair> Sides = new ArrayList<Pair>();//внесём в список все клетки, расположенные вокруг нынешней
            Sides.add(new Pair(x, y - 1));
            Sides.add(new Pair(x, y + 1));
            Sides.add(new Pair(x + 1, y));
            Sides.add(new Pair(x - 1, y));
            for (Pair side : Sides) {
                if (isAvailable(side, Lab)) { //клетка доступна
                    if (Pair.eq(side, finish)) {
                        return steps + 1;//если вдруг пробрались таки до финиша - сигнализируем и возвращаем количество шагов до него
                    }
                    int xt = side.getX();
                    int yt = side.getY();
                    if ((Lab[xt][yt] == 0) || (Lab[xt][yt] > steps + 1)) {//длина пути в эту клетку неизвестна
                        Lab[xt][yt] = steps + 1;                    //или превышает новую найденную
                        if (!Outp.contains(new Pair(xt, yt)))
                            Outp.add(new Pair(xt, yt));
                    }
                }
            }
        }
        return findWay((steps+1), Outp, Lab, finish);
    }


    //проверяем, находится ли данная клетка в пределах лабиринта и нет ли в ней стенки
    static boolean isAvailable (Pair pair, int[][] Lab) {
        if (pair.getX() < 0)
            return false;
        if (pair.getX() >= width)
            return false;
        if (pair.getY() < 0)
            return false;
        if (pair.getY() >= height)
            return false;
        if (Lab[pair.getX()][pair.getY()] == -1)
            return false;
        return true;
    }

    public static void main(String[] args) {
        final int[][] Lab = new int[height][width];
        final Pair start = new Pair(0, 9);
        Lab[start.getX()][start.getY()] = -2;
        final Pair finish = new Pair(9, 0);
        Lab[finish.getX()][finish.getY()] = -2;
        for (int i = 0; i < height - 1; i++) {//выстроим пару стенок внутри лабиринта
        Lab[i][5] = -1;
        Lab[i + 1][3] = -1;
        }
        System.out.print("Координаты стартовой точки (нумерация с 0):");
        start.printPair();
        System.out.print("Координаты финишной точки (нумерация с 0):");
        finish.printPair();
        System.out.println("Схематичное обозначение нашего лабиринта\n-1 означает, что клетка занята стенкой\n-2 обозначает старт и финиш");
        printMat(Lab);
        if (Pair.eq(start, finish))
            System.out.println("Длина пути: 0, старт с финишем совпадают");
        else {
            List<Pair> startList = new ArrayList<Pair>();
            startList.add(start);
            int n = findWay(0, startList, Lab, finish);
            System.out.println("Длина пути: " + n);
        }
    }
}
