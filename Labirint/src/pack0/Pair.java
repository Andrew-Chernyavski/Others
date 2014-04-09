package pack0;

/**
 * Created by Andrew on 09.04.14.
 */
public class Pair {
    final int x;
    final int y;

    public Pair(int t, int u) {
        this.x= t;
        this.y= u;
    }

    public void printPair() {
        System.out.println("(" + this.x + ", " + this.y + ")");
    }

    public int getX() {
        return this.x;
    }

    public int getY() {
        return this.y;
    }

    public static boolean eq(Pair a, Pair b) {
        return ((a.getX() == b.getX()) && (a.getY() == b.getY()));
    }
}
