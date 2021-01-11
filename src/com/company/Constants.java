package com.company;

public class Constants {
    private final float Q = 10;
    private final float Ro = 0.8f;
    private final float alpha = 1;
    private final float beta = 1;
    private final int iterationNumber = 50;
    private final int minimum = 1000000;

    public float getQ() {
        return Q;
    }

    public float getRo() {
        return Ro;
    }

    public float getAlpha() {
        return alpha;
    }

    public float getBeta() {
        return beta;
    }

    public int getIterationNumber() {
        return iterationNumber;
    }

    public int getMinimum() {
        return minimum;
    }

    public static int Newton(int n, int k)
    {
        if (k == 0 || k == n)
            return 1;
        else
            return Newton(n-1, k-1) + Newton(n-1, k);
    }

    public static int getRandomNumber(int min, int max) {
        return (int) ((Math.random() * (max - min)) + min);
    }

    public static float dist(Point p1, Point p2){
        return (float) Math.sqrt(Math.pow(p2.getX() - p1.getX(), 2) + Math.pow(p2.getY() - p1.getY(), 2));
    }

    public static int calculateCombinationIndex(int pointIndex, int n, int i){
        int ind;
        int start = n - Math.min(pointIndex, i);
        int stop = n;
        int sum = 0;
        while (start < stop){
            sum += start;
            start++;
        }
        ind = Math.abs(pointIndex - i) - 1 + sum;
        return ind;
    }
}
