package com.company;
import java.lang.Math;
import java.util.ArrayList;

import static com.company.Constants.getRandomNumber;

public class Point {
    private int index;
    private int x;
    private int y;
    private boolean visited;

    public Point(int index, int x, int y, boolean visited) {
        this.index = index;
        this.x = x;
        this.y = y;
        this.visited = visited;
    }

    public int getIndex() {
        return index;
    }

    public void setIndex(int index) {
        this.index = index;
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

    public boolean isVisited() {
        return visited;
    }

    public void setVisited(boolean visited) {
        this.visited = visited;
    }

    public ArrayList<Point> generatePoints(int n){
        ArrayList<Point> points = new ArrayList<Point>();
        int index, x, y;
        Point point;
        for (int i = 0; i < n; i++) {
            index = i + 1;
            x = getRandomNumber(0, n * 10);
            y = getRandomNumber(0, n * 10);
            point = new Point(index, x, y, false);
            points.add(point);
        }
        return points;
    }
}
