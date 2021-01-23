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
        map<pair<int,int>,float> pheromone_map; //pheromones produced by one ant
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
    generate_pheromones(points, n, ants);
    return ants;
}
bool check_if_visited(Ant ant, int point_index){
    for(int i = 0; i < ant.visited.size(); i++){
        if (ant.visited[i] == point_index){
            return true;
        }
    }
    return false;
}

//function that calculates distances between all cities
map<pair<int,int>,float> calculate_point_distances(vector<Point> points, int n){ 
    float distance;
    map<pair<int,int>,float> point_distances;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            pair<int, int> point_index_pair1(points[i].index, points[j].index);  //index pairs
            pair<int, int> point_index_pair2(points[j].index, points[i].index); //reverse index pairs
            distance = dist(points[i], points[j]);
            point_distances[point_index_pair1] = distance;
            point_distances[point_index_pair2] = distance;
        }
    }
    return point_distances;
}

//function that generates pheromones map
void generate_pheromones(vector<Point> points, int n, vector<Ant> ants){
    float pheromones;
    map<pair<int,int>,float> pheromone_map;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            pair<int, int> point_index_pair1(points[i].index, points[j].index);  //index pairs
            pair<int, int> point_index_pair2(points[j].index, points[i].index); //reverse index pairs
            pheromones = 1;
            pheromone_map[point_index_pair1] = pheromones;
            pheromone_map[point_index_pair2] = pheromones;
        }
    }
    for(Ant ant : ants){
        ant.pheromone_map = pheromone_map;
    }
}


//function that calculates probabilities
void calculate_probabilities(Ant ant, map<pair<int,int>,float> point_distances, int n, Constants c){
    ant.probabilities = vector<float>(n, 0.0); //clear probability vector
    float probability, probability_n, probability_d;
    for(int i = 0; i < n; i++){
        if(check_if_visited(ant, i)){
            ant.probabilities[i] = 0; //probability equals 0 if city is visited
        }
        else{ //probability is calculated using the formula (1)
            probability_d = 0;
            pair<int, int> p(ant.point_index, i);
            probability_n = pow(ant.pheromone_map[p],c.alpha) * pow(1/point_distances[p],c.beta);
            for(int j = 0; j < n; j++){
                pair<int, int> p_d(ant.point_index, j);
                probability_d += pow(ant.pheromone_map[p_d],c.alpha) * pow(1/point_distances[p_d],c.beta);
            }
            ant.probabilities[i] = probability_n / probability_d;
        }
    }
}

//function that selects next city
int select_next_city(Ant ant, int n){
    float random = ((double) rand() / (RAND_MAX));
    int selected;
    for(int i = 0; i < n; i++){
        random -= ant.probabilities[i];
        if (random <= 0){
            return i;
        }
    }
    return -15; //error return
}

map<pair<int,int>, float> update_trail_levels(Ant ant, map<pair<int,int>,float> pheromones_map,  map<pair<int,int>,float> point_distances, Constants c){
    float Lk = ant.Lk;
    for(int i = 1; i < ant.visited.size() - 1; i++){
        pair<int, int> p(ant.visited[i-1], ant.visited[i]);
        pheromones_map[p] += c.Q/Lk;
    }
    return pheromones_map;
}

float calculate_Lk(Ant ant, map<pair<int,int>,float> point_distances, int n){
    float Lk = 0;
    // for(int i = 1; i < ant.visited.size() - 1; i++){
    //     pair<int, int> p(ant.visited[i-1], ant.visited[i]);
    //     Lk += point_distances[p];
    // }
    for(int i = 1; i < n - 1; i++){
        pair<int, int> p(ant.visited[i-1], ant.visited[i]);
        Lk += point_distances[p];
    }
    return Lk;
}

map<pair<int,int>, float> calculate_total_pheromones(vector<Ant> ants, int n){
    map<pair<int,int>, float> total_pheromones_map;
    for(Ant ant : ants){
        for(int i = 0; i < n; i++){
            for(int j = i + 1; j < n; j++){
                pair<int,int> p1(i,j);
                pair<int,int> p2(j,i);
                total_pheromones_map[p1] += ant.pheromone_map[p1];
                total_pheromones_map[p2] += ant.pheromone_map[p2];
            }
        }
    }
    return total_pheromones_map;
}


int main(){
    cout << "Manual input:" << endl;
    int n, index, x, y;
    int iteration_number = 50;
    int minimum = 1000000;
    cin >> n;
    Constants c;
    // vector<float> pheromones = vector<float>(newton(n, 2), 1.0);
    vector<Point> points;
    vector<Ant> ants;

    for (int i = 0; i < n; i++){
        cin >> index >> x >> y;
        points.push_back(Point(index-1, x, y));
    }

    map<pair<int,int>,float> point_distances = calculate_point_distances(points, n); //distances between all cities

    //main loop
    for (int t = 0; t < iteration_number; t++){
        //ant loop
        for (Ant ant : ants){
            //ant's move loop
            for(int move; move < n; move++){
                calculate_probabilities(ant, point_distances, n, c);
                select_next_city(ant, n);
            }
            ant.Lk = calculate_Lk(ant, point_distances, n);
        }

    }
}
