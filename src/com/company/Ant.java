package com.company;

import java.util.ArrayList;
import java.util.Collections;

import static com.company.Constants.getRandomNumber;
import static com.company.Constants.Newton;

public class Ant {
    private int pointIndex;
    private ArrayList<Integer> visitedIndex = new ArrayList<Integer>();
    private ArrayList<Float> probabilities = new ArrayList<Float>();
    private float Lk;
    private ArrayList<Float> combinations = new ArrayList<Float>();

    public Ant(int pointIndex) {
        this.pointIndex = pointIndex;
    }

    public int getPointIndex() {
        return pointIndex;
    }

    public void setPointIndex(int pointIndex) {
        this.pointIndex = pointIndex;
    }

    public ArrayList<Integer> getVisitedIndex() {
        return visitedIndex;
    }

    public void setVisitedIndex(ArrayList<Integer> visitedIndex) {
        this.visitedIndex = visitedIndex;
    }

    public void addVisited(int pointIndex){
        this.visitedIndex.add(pointIndex);
    }

    public ArrayList<Float> getProbabilities() {
        return probabilities;
    }

    public void setProbabilities(ArrayList<Float> probabilities) {
        this.probabilities = probabilities;
    }

    public void addProbabilities(float probability){
        this.probabilities.add(probability);
    }

    public float getLk() {
        return Lk;
    }

    public void setLk(float lk) {
        Lk = lk;
    }

    public ArrayList<Float> getCombinations() {
        return combinations;
    }

    public void setCombinations(ArrayList<Float> combinations) {
        this.combinations = combinations;
    }

    public void setCombinationIndex(int index, float value){
        this.combinations.add(index, value);
    }


    public static ArrayList<Ant> spawnAnts(int n, ArrayList<Point> points){
        ArrayList<Ant> ants = new ArrayList<Ant>();
        int spawnPoint;
        int combinations;
        Ant ant;

        for (int i = 0; i < 2 * n; i++) {
            spawnPoint = getRandomNumber(0, n - 1);
            ant = new Ant(spawnPoint);
            combinations = Newton(n, 2);
            for (int j = 0; j < combinations; j++) {
                ant.combinations.add(0f);
            }
            for (int j = 0; j < n; j++) {
                ant.probabilities.add(0f);
            }
            ant.setLk(0);
            ants.add(ant);
        }
        return ants;
    }
}
