#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>


using namespace std;

class Point{
    public:
        int index;
        int x;
        int y;

        Point(){};
        Point(int index, int x, int y){
            this->index = index;
            this->x = x;
            this->y = y;
        };
};

class Ant{
    public:
        int point_index;
        vector<int> visited; //visited cities
        vector<float> probabilities; //probabilities of going to each city
        float Lk; //lenght of the tour

        Ant(){};
        Ant(int point_index){
            this->point_index = point_index;
        };
};

class Constants{
    public:
        const float Q = 10;
        const float Ro = 0.8;
        const float alpha = 1;
        const float beta = 1;

        Constants(){};
};

float dist(Point p1, Point p2){
    return sqrt(pow(p2.x - p1.x,2) + pow(p2.y - p1.y, 2));
}

int newton(int n, int k) {
   if (k == 0 || k == n)
   return 1;
   return newton(n - 1, k - 1) + newton(n - 1, k);
}

vector<Point> generate_points(int n){
    vector<Point> points;
    int index, x, y;
    for (int i = 0; i < n; i++){
        index = i;
        x = rand()%(n*10 + 1);
        y = rand()%(n*10 + 1);
        points.push_back(Point(index, x, y));
    }
    return points;
}

//function that randomly spawns ants across the cities
vector<Ant> spawn_ants(int n, vector<Point> points){
    vector<Ant> ants;
    int spawn_point_index;
    for (int i = 0; i < 2*n; i++){
        spawn_point_index = rand()%((n-1) + 1);
        Ant ant(spawn_point_index);
        ant.visited.push_back(spawn_point_index);
        ants.push_back(ant);
    }
    return ants;
}

//function that calculates distances between all cities
map<pair<int,int>,float> calculate_point_distances(vector<Point> points){ 
    pair<int, int> point_index_pair;
    float distance;
    map<pair<int,int>,float> point_distances;

    for(int i = 0; i < points.size(); i++){
        for(int j = i + 1; j < points.size(); j++){
            point_index_pair.first = points[i].index;
            point_index_pair.second = points[j].index;
            distance = dist(points[i], points[j]);
            point_distances[point_index_pair] = distance;
        }
    }
    return point_distances;
}

// float calculate_probability(){
//     float probability = 1;

//     return probability;
// }


int main(){
    cout << "Manual input:" << endl;
    int n, index, x, y;
    int iteration_number = 50;
    int minimum = 1000000;
    cin >> n;
    Constants c;
    vector<float> pheromones = vector<float>(newton(n, 2), 1.0);
    vector<Point> points;
    vector<Ant> ants;

    for (int i = 0; i < n; i++){
        cin >> index >> x >> y;
        points.push_back(Point(index-1, x, y));
    }

    map<pair<int,int>,float> point_distances = calculate_point_distances(points);

    pair<int,int> p(1,3);
    cout << point_distances[p];
}