package com.company;

import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

import static com.company.Constants.*;

public class MainBackup {

    public static void main(String[] args) {

//        print("Choose input type:\n1 - Generated\n2 - Manual input")
//        inp = int(input())
//
//        if inp == 1:
//        print("Generated input:")
//        print("Enter point number")
//        n = int(input())
//        points = generate_points(n)
//        write_results(points)
//        elif inp == 2:
//        print("Manual input:")
//        n = int(input())
//        points = []
//        for i in range(n):
//        index, x, y = input().split()
//        point = Point(int(index), float(x), float(y), False)
//        points.append(point)
//    else:
//        print("Wrong number.")
//        return 0

        Scanner input = new Scanner(System.in);

        System.out.println("Choose input type:\n1 - Generated\n2 - Manual input");
        int choice = input.nextInt();
        int n = 0;
        ArrayList<Point> points = new ArrayList<Point>();
        ArrayList<Ant> ants = new ArrayList<Ant>();
        Constants c = new Constants();
        ArrayList<Float> pheromoneSums = new ArrayList<Float>();

        if (choice == 1){
            System.out.println(choice);
        }

        else if (choice == 2){
            System.out.println("Manual input:");
            n = input.nextInt();
            Point point;
            for (int i = 0; i < n; i++) {
                int[] line = new int[3];
                for (int j = 0; j < 3; j++){
                    line[j] = input.nextInt();
                }
                int index = line[0];
                int x = line[1];
                int y = line[2];
                point = new Point(index, x, y, false);
                points.add(point);
            }

        }
        else System.out.println("Wrong number.");

        ants = Ant.spawnAnts(n, points);
        ArrayList<Float> pheromones = new ArrayList<Float>();
        float minimum = c.getMinimum();

        for (int i = 0; i < Newton(n, 2); i++) {
            pheromones.add(1f);
        }

        for (int t = 0; t < c.getIterationNumber(); t++) {
            for (int i = 0; i < Newton(n, 2); i++) {
                pheromoneSums.add(0f);
            }
            for (Ant ant : ants){
                ant.setLk(0);
                ant.addVisited(ant.getPointIndex());
                ArrayList<Integer> notVisited = new ArrayList<Integer>();
                for (int move = 0; move < n - 1; move++) {
                    notVisited.clear();
                    for (int i = 0; i < n; i++) {
                        if (!ant.getVisitedIndex().contains(points.get(i).getIndex())){
                            notVisited.add(points.get(i).getIndex());
                        }
                    }
                    for (int i = 0; i < n; i++) {
                        int ind;
                        float numerator, denominator, p;
                        if (!ant.getVisitedIndex().contains(i)){
                            ind = Constants.calculateCombinationIndex(ant.getPointIndex(), n, i);
//                            numerator = pheromones[ind] ** Constants.alfa * 1/dist(points[ant.point], points[i])
//                            denominator = 0
                            numerator = (float) Math.pow(pheromones.get(ind), c.getAlpha() * 1/dist(points.get(ant.getPointIndex()), points.get(i)));
                            denominator = 0;
//                            for j in range(len(not_visited)):
//                                ind_d = abs(ant.point - not_visited[j]) - 1 + sum(range(n - min(ant.point, not_visited[j]), n))
//                                denominator += pheromones[ind_d] ** Constants.alfa * 1/dist(points[ant.point], points[not_visited[j]])
                            int ind_d;
                            for (int j = 0; j < notVisited.size(); j++) {
                                ind_d = Constants.calculateCombinationIndex(ant.getPointIndex(), n, notVisited.get(j));
                                denominator += Math.pow(pheromones.get(ind_d), c.getAlpha()) * 1/dist(points.get(ant.getPointIndex()), points.get(notVisited.get(j)));
                            }
                            p = numerator / denominator;
                        }
                        else p = 0;
                        ant.addProbabilities(i);
                        float rand = (float) Math.random();
                        int selected = 0;
                        for (int j = 0; j < ant.getProbabilities().size(); j++) {
                            rand -= ant.getProbabilities().get(j);
                            if (rand <= 0){
                                selected = j;
                                break;
                            }
                        }
                        Point pointI = points.get(ant.getPointIndex());
                        Point pointJ = points.get(selected);
                        ant.setLk(ant.getLk() + dist(pointI, pointJ));
                        ant.addVisited(selected);
                        ind = calculateCombinationIndex(pointJ.getIndex()-1, n, pointI.getIndex()-1);
                        ant.setCombinationIndex(ind, c.getQ());
                        ArrayList<Float> probabilities = new ArrayList<Float>();
                        for (int j = 0; j < n; j++) {
                            probabilities.add(0f);
                        }
                        ant.setProbabilities(probabilities);
                    }
                    float d = dist(points.get(ant.getVisitedIndex().get(ant.getVisitedIndex().size()-1)), points.get(ant.getVisitedIndex().get(0)));
                    ant.setLk(ant.getLk() + d);
                    int ind = calculateCombinationIndex(ant.getVisitedIndex().get(ant.getVisitedIndex().size()-1), ant.getVisitedIndex().get(0), n);
                    ant.setCombinationIndex(ind, c.getQ());
                    if (ant.getLk() < minimum){
                        minimum = ant.getLk();
                    }
                    for (int i = 0; i < ant.getCombinations().size(); i++) {
                        ant.setCombinationIndex(i, ant.getCombinations().get(i)/ant.getLk());
                    }
                }
            }
            for (int i = 0; i < Newton(n, 2); i++) {
                for (Ant ant : ants){
                    pheromoneSums.add(i, ant.getCombinations().get(i));
                }
            }
            for (int i = 0; i < pheromoneSums.size(); i++) {
                pheromones.add(i, pheromones.get(i) * c.getRo() + pheromoneSums.get(i));
            }
        }


    }
}
