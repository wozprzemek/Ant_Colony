#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <chrono>
#include <windows.h>
#include <set>



using namespace std;

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart) / 1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

class Constants {
public:
    const float Q = 10;
    const float Ro = 0.8;
    const float alpha = 1;
    const float beta = 1;

    Constants() {};
};

class City {
public:
    int index;
    pair<double, double> coordinates;

    City(int index, int x, int y) {
        this->index = index;
        this->coordinates = pair<double, double>(x, y);
    }
};

class Ant {
public:
    int current_city_index;
    vector <int> path;
    set<int> not_visited;
    double Lk;
    Constants c;

    Ant(/*int current_city*/ int n) {
        this->current_city_index = 0; //setting the starting point as 1 (for now) ->[spawn_ants]
        this->path.push_back(0);

        //filling the not_visited set, starting from the point after 1 (for now) ->[spawn_ants]
        for (int i = 1; i < n; i++) {
            not_visited.insert(i);
        }
    }

    void reset(int n) {
        this->current_city_index = 0;
        this->path.clear();
        this->path.push_back(current_city_index);
        this->not_visited.clear();
        for (int i = 1; i < n; i++) {
            not_visited.insert(i);
        }

    }

    void move(vector<vector<double>> T, vector<vector<double>> city_visibilities) {
        double random = ((double)rand() / (RAND_MAX));
        double probability;
        double p_n; //numerator probability
        double p_d = 0.0; //denominator probability
        double sum = 0;
        // cout << "random " << random << endl;
        // cout << "not visited size " << not_visited.size() << endl;
        for (set<int>::iterator i = not_visited.begin(); i != not_visited.end(); i++) { //iterate over not visited cities
            p_d = 0.0;
            /*PART 1--------calculate probability for a city-----------*/
            p_n = (pow(T[current_city_index][*i], c.alpha)) * (pow(city_visibilities[current_city_index][*i], c.beta)); //numerator probability
            for (set<int>::iterator j = not_visited.begin(); j != not_visited.end(); j++) {
                p_d += (pow(T[current_city_index][*j], c.alpha)) * (pow(city_visibilities[current_city_index][*j], c.beta)); //denominator probability
            }
            probability = p_n / p_d;
            // cout << " probability " << probability << endl;
            // cout << "random " << random << " prob " << probability << endl;

            /*PART 2--------try to select the city for which the probability was calculated-----------*/
            random -= probability;
            if (random <= 0) {
                // cout << "random after subtraction " << random << endl;
                current_city_index = *i;
                path.push_back(current_city_index); //add selected city to ant's path
                not_visited.erase(i); //remove selected city from not visited
                break;
            }
        }
    }

};

double dist(City p1, City p2) {
    return sqrt(pow(p2.coordinates.first - p1.coordinates.first, 2) + pow(p2.coordinates.second - p1.coordinates.second, 2));
}

double calculate_lk(Ant ant, vector<vector<double>> point_distances, int n) {
    float Lk = 0;
    for (int i = 1; i < n + 1; i++) {
        Lk += point_distances[ant.path[i - 1]][ant.path[i]];
        // cout << point_distances[i-1][i] << " ";
    }
    // cout << endl;
    return Lk;
}

vector<vector<double>> update_trail_levels(Ant ant, double Lk, vector<vector<double>> T, Constants c, int n) {
    vector<vector<double>> pheromone_levels = T;
    for (int i = 1; i < n + 1; i++) { //add the pheromones along the ant's path
        pheromone_levels[ant.path[i - 1]][ant.path[i]] += c.Q / Lk;
    }

    return pheromone_levels;
}

vector<vector<double>> evaporate_trail_levels(vector<vector<double>> T, Constants c, int n) {
    vector<vector<double>> pheromone_levels = T;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pheromone_levels[i][j] = pheromone_levels[i][j] * c.Ro;  //evaporate the pheromones on all possible paths
        }
    }

    return pheromone_levels;
}

vector<vector<double>> calculate_city_visibilities(vector<City> cities, int n) {
    vector<vector<double>> city_visibilities;
    vector<double> row;
    double visibility, distance;
    for (int i = 0; i < n; i++) {
        row.clear();
        for (int j = 0; j < n; j++) {
            distance = dist(cities[i], cities[j]);
            if (distance != 0) { //if distance is 0, set visibility as -1
                visibility = 1 / distance;
            }
            else {
                visibility = -1;
            }

            row.push_back(visibility);
        }
        city_visibilities.push_back(row);
    }
    return city_visibilities;
}

vector<vector<double>> calculate_city_distances(vector<City> cities, int n) {
    vector<vector<double>> city_distances;
    vector<double> row;
    double distance;
    for (int i = 0; i < n; i++) {
        row.clear();
        for (int j = 0; j < n; j++) {
            distance = dist(cities[i], cities[j]);
            row.push_back(distance);
        }
        city_distances.push_back(row);
    }
    return city_distances;
}

vector<vector<double>> initialize_pheromones(int n) {
    vector<vector<double>> pheromones;
    vector<double> row;
    for (int i = 0; i < n; i++) {
        row.clear();
        for (int j = 0; j < n; j++) {
            row.push_back(1.0);
        }
        pheromones.push_back(row);
    }
    return pheromones;
}

vector<Ant> spawn_ants(int n) {
    vector<Ant> ants;
    for (int i = 0; i < n; i++) {
        Ant ant(n);
        ants.push_back(ant);
    }
    return ants;
}


int main() {
    srand(time(NULL));
    double minimum = 1000000;
    int iteration_number = 2000;
    vector<int> path;

    cout << "Manual input:" << endl;
    int n, index;
    double x, y;
    cin >> n;
    Constants c;
    vector<City> cities;
    int city_number = n;
    int ant_number = n;
    double Lk;
    vector<int> shortest_path;

    //read input data
    for (int i = 0; i < n; i++) {
        cin >> index >> x >> y;
        cities.push_back(City(index - 1, x, y));
    }

    vector<vector<double>> city_distances = calculate_city_distances(cities, n); //initialize city distances table
    vector<vector<double>> city_visibilities = calculate_city_visibilities(cities, n); //initialize city visibilities table
    vector<vector<double>> T = initialize_pheromones(n); //initialize pheromones table

    /* ------test city visibilities------ */
    // for (int i=0; i<n; i++){
    //     for (int j=0; j<n; j++){
    //         cout << city_visibilities[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    /* ------test city distances------ */
    // for (int i=0; i<n; i++){
    //     for (int j=0; j<n; j++){
    //         cout << city_distances[i][j] << " ";
    //     }
    //     cout << endl;
    // }


    /* ------test pheromones------ */
    // for (int i=0; i<n; i++){
    //     for (int j=0; j<n; j++){
    //         cout << T[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    vector<Ant> ants = spawn_ants(n); //spawn all ants

    //MAIN LOOP
    for (int t = 0; t < iteration_number; t++) {
        cout << "Iteration: " << t << endl;
        for (int city = 0; city < n; city++) { //iterate over cities
            for (int l = 0; l < ant_number; l++) { //iterate over ants
                ants[l].move(T, city_visibilities); //move each ant
            }
        }

        for (int l = 0; l < ant_number; l++) { //iterate over ants
            ants[l].path.push_back(0); //return to spawn city
            Lk = calculate_lk(ants[l], city_distances, n); //calculate the total distance for ant
            T = update_trail_levels(ants[l], Lk, T, c, n); //add the pheromones along the ant's path
            if (Lk < minimum) {
                minimum = Lk; //check if the path is the shortest yet
                // shortest_path = ants[l].path;
            }
            ants[l].reset(n);

        }
        T = evaporate_trail_levels(T, c, n); //evaporate all pheromones
        // cout << "Shortest path: ";
        // for (int i=0; i<n; i++){
        //     cout << shortest_path[i] << " ";
        // }
        // cout << endl;
        /* ------test pheromones------ */
        // for (int i = 0; i < n; i++) {
        //     for (int j = 0; j < n; j++) {
        //         cout << T[i][j] << " ";
        //     }
        //     cout << endl;
        // }
    }
    cout << "Minimum: " << minimum << endl;

}