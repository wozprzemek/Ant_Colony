#include <iostream>
#include <cmath>
#include <vector>

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
        vector<int> visited;
        vector<float> visited;
        float Lk;
        vector<float> combinations;

        Ant(){};
        Ant(int point_index){
            this->point_index = point_index;
        };
};

class Constants{
    const float Q = 10;
    const float Ro = 0.8;
    const float alpha = 1;
    const float beta = 1;
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

    for (int i = 0; i < count; i++){
        /* code */
    }
    
}

