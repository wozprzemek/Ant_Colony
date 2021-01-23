#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Point {
public:
    int index;
    int x;
    int y;

    Point() {};
    Point(int index, int x, int y) {
        this->index = index;
        this->x = x;
        this->y = y;
    };
};

class Ant {
public:
    int point_index;
    vector<int> visited;
    float Lk;
    vector<float> probabilities;
    vector<float> combinations;

    Ant() {};
    Ant(int point_index) {
        this->point_index = point_index;
    };
};

class Constants {
public:
    const float Q = 10;
    const float Ro = 0.8;
    const float alpha = 1;
    const float beta = 1;

    Constants() {};
};

float dist(Point p1, Point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

int newton(int n, int k) {
    if (k == 0 || k == n)
        return 1;
    return newton(n - 1, k - 1) + newton(n - 1, k);
}

vector<Point> generate_points(int n) {
    vector<Point> points;
    int index, x, y;
    for (int i = 0; i < n; i++) {
        index = i;
        x = rand() % (n * 10 + 1);
        y = rand() % (n * 10 + 1);
        points.push_back(Point(index, x, y));
    }
    return points;
}

int calculate_combination_index(int pointIndex, int destinationIndex, int n) {
    int ind;
    int start = n - min(pointIndex, destinationIndex);
    int stop = n;
    int sum = 0;
    while (start < stop) {
        sum += start;
        start++;
    }
    ind = abs(pointIndex - destinationIndex) - 1 + sum;
    return ind;
}

vector<Ant> spawn_ants(int n, vector<Point> points) {
    vector<Ant> ants;
    int spawn_point_index;
    for (int i = 0; i < 2 * n; i++) {
        spawn_point_index = rand() % ((n - 1) + 1);
        Ant ant(spawn_point_index);
        ant.visited.push_back(spawn_point_index);
        ant.probabilities = vector<float>(n, 0.0);
        ant.combinations = vector<float>(newton(n, 2), 0.0);
        ant.Lk = 0;
        ants.push_back(ant);
    }
    return ants;
}

int main() {
    srand(time(NULL));
    cout << "Manual input:" << endl;
    int n, index;
    float x, y;
    int iteration_number = 50;
    int minimum = 1000000;
    cin >> n;
    Constants c;
    vector<float> pheromones = vector<float>(newton(n, 2), 1.0);
    vector<Point> points;
    vector<Ant> ants;

    for (int i = 0; i < n; i++) {
        cin >> index >> x >> y;
        points.push_back(Point(index - 1, x, y));
    }

    ants = spawn_ants(n, points);

    for (int t = 0; t < iteration_number; t++) {
        cout << "t " << t << endl;
        vector<float> pheromone_sums = vector<float>(newton(n, 2), 0.0);
        float p;
        int selected;
        for (Ant ant : ants) {
            ant.Lk = 0;
            ant.visited.clear();
            ant.visited.push_back(ant.point_index);
            vector<int> not_visited;
            for (int move = 0; move < n - 1; move++) {
                // cout << "   move: " << move << endl;
                not_visited.clear();
                for (int i = 0; i < n; i++) {
                    if (find(ant.visited.begin(), ant.visited.end(), points[i].index) != ant.visited.end()) {}
                    else {
                        not_visited.push_back(points[i].index);
                    }
                }
                for (int i = 0; i < n; i++) {
                    if (find(ant.visited.begin(), ant.visited.end(), i) != ant.visited.end()) {
                        p = 0;
                    }
                    else {
                        int ind = calculate_combination_index(ant.point_index, i, n);
                        float numerator = pow(pheromones[ind], c.alpha) * 1 / dist(points[ant.point_index], points[i]);
                        float denominator = 0;
                        for (int j = 0; j < not_visited.size(); j++) {
                            int ind_denominator = calculate_combination_index(ant.point_index, not_visited[j], n);
                            denominator += pow(pheromones[ind_denominator], c.alpha) * 1 / dist(points[ant.point_index], points[not_visited[j]]);
                        }
                        p = numerator / denominator;
                    }
                    ant.probabilities[i] = p;
                }
                float random = ((double)rand() / (RAND_MAX));
                for (int i = 0; i < ant.probabilities.size(); i++) {
                    random -= ant.probabilities[i];
                    if (random <= 0) {
                        selected = i;
                        break;
                    }
                }
                Point point_i = points[ant.point_index];
                Point point_j = points[selected];
                ant.Lk += dist(point_i, point_j);
                ant.point_index = selected;
                ant.visited.push_back(selected);
                int ind = calculate_combination_index(point_i.index, point_j.index, n);
                ant.combinations[ind] = c.Q;
                ant.probabilities = vector<float>(n, 0.0);
            }
            float distance = dist(points[ant.visited[ant.visited.size() - 1]], points[ant.visited[0]]);
            ant.Lk += distance; //??????????
            int ind = calculate_combination_index(ant.visited[ant.visited.size() - 1], ant.visited[0], n);
            ant.combinations[ind] = c.Q;
            if (ant.Lk < minimum) {
                minimum = ant.Lk;
            }
            for (int i = 0; i < ant.combinations.size(); i++) {
                ant.combinations[i] = ant.combinations[i] / ant.Lk;
            }
        }
        for (int i = 0; i < newton(n, 2); i++) {
            for (auto ant : ants) {
                pheromone_sums[i] += ant.combinations[i];
            }
        }
        for (int i = 0; i < pheromone_sums.size(); i++) {
            pheromones[i] = pheromones[i] * c.Ro + pheromone_sums[i];
        }
    }
    cout << "Min: " << minimum << endl;


}

