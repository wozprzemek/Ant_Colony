# Ant_Colony

Ant Colony Optimization for generalized TSP problem. Implemented in C++ as a part of a group university project.

## Theory
Ant Colony Optimization (ACO) is an method of solving the Traveling Salesman Problem (TSP).
For more detailed information, see https://www.sciencedirect.com/science/article/pii/S1002007108002736.
For an in-depth overview and performance analysis of the implementation see the documentation located in this repository.

## Getting started
This implementation of the ACO algorithm is written in C++. To run the application on your computer you need a C++ compiler.

## Setup
Clone the repository to your local machine or download the source file.
You can either compile it directly from the terminal (-O3 optimization flag reccomended) and then run the output file or run the application through and IDE of your choice that supports C++ compilation.

## Usage
Once you have managed to run the application you will be asked to input the TSP data.
The input format looks like the following:
```
n of points
1 x_coord y_coord
2 x_coord y_coord
3 x_coord y_coord
...
n x_coord y_coord
```
The program shows the current iteration of the algorithm. After it has finished, you will be shown the result of the algorithm.
You can find example instances for the TSP problem along with the best known results here: http://elib.zib.de/pub/mp-testdata/tsp/tsplib/tsplib.html.
