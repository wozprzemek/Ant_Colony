import math
import random
import matplotlib.pyplot as plt
from scipy.special import comb
import itertools


class Point:
    def __init__(self, index, x, y, visited):
        self.index = index
        self.x = x
        self.y = y
        self.visited = visited


class Ant:
    def __init__(self, point):
        self.point = point
        self.visited = []
        self.probabilities = []
        self.Lk = 0
        self.combinations = []


class Constants:
    Q = 10
    Ro = 0.8
    alfa = 1
    beta = 1


def dist(p1,p2):
    return math.sqrt((p2.x - p1.x)**2 + (p2.y - p1.y)**2)


def generate_points(n):
    points = []
    for i in range(n):
        index = i + 1
        x = random.randint(0, n*10)
        y = random.randint(0, n*10)
        point = Point(int(index), float(x), float(y), False)
        points.append(point)
    # for i in range(len(points)):
    #     for j in range(i+1, len(points)):
    #         print(i, j, 1/(dist(points[i], points[j])))
    return points


def generate_plot(points):
    x = []
    y = []
    for point in points:
        x.append(point.x)
        y.append(point.y)
    fig, ax = plt.subplots()
    ax.scatter(x, y)
    for i in range(len(x)):
        ax.annotate(i+1, (x[i], y[i]),fontsize=16)

    for i in range(len(x)):
        for j in range(i+1,len(y)):
            distance = math.sqrt((x[i] - x[j])**2 + (y[i] - y[j])**2)
            plt.plot([x[i],x[j]], [y[i],y[j]], label=distance)
    plt.show()


def write_results(points):
    file = open('input.txt', 'w')
    file.write(str(len(points)))
    file.write('\n')
    for point in points:
        res = str(point.index) + ' ' + str(point.x) + ' ' + str(point.y)
        file.write(res)
        file.write('\n')
    file.close()


def spawn_ants(n, points):
    ants = []
    for i in range(2*n):
        spawn_point = random.randint(0, n-1)
        # combinations = list(itertools.combinations(points,2))
        ant = Ant(spawn_point)
        ant.visited.append(spawn_point)
        ant.combinations = [0] * int(comb(n, 2))
        ant.probabilities = [0] * n
        ant.Lk = 0
        ants.append(ant)
    return ants


def main():
    print("Choose input type:\n1 - Generated\n2 - Manual input")
    inp = int(input())

    if inp == 1:
        print("Generated input:")
        print("Enter point number")
        n = int(input())
        points = generate_points(n)
        write_results(points)
    elif inp == 2:
        print("Manual input:")
        n = int(input())
        points = []
        for i in range(n):
            index, x, y = input().split()
            point = Point(int(index), float(x), float(y), False)
            points.append(point)
    else:
        print("Wrong number.")
        return 0

    ants = spawn_ants(n, points)
    iteration_number = 50

    pheromones = [1] * int(comb(n, 2))
    minimum = 1000000

    for t in range(iteration_number):
        pheromone_sums = [0] * int(comb(n, 2))
        for ant in ants:
            ant.Lk = 0
            ant.visited = []
            ant.visited.append(ant.point)
            for move in range(n-1):
                not_visited = [item for item in range(n) if item not in ant.visited]
                # print(not_visited)
                for i in range(n):
                    if i not in ant.visited:
                        ind = abs(ant.point - i) - 1 + sum(range(n - min(ant.point, i), n))
                        numerator = pheromones[ind] ** Constants.alfa * 1/dist(points[ant.point], points[i])
                        denominator = 0
                        for j in range(len(not_visited)):
                            ind_d = abs(ant.point - not_visited[j]) - 1 + sum(range(n - min(ant.point, not_visited[j]), n))
                            denominator += pheromones[ind_d] ** Constants.alfa * 1/dist(points[ant.point], points[not_visited[j]])
                        p = numerator / denominator
                    else:
                        p = 0
                    ant.probabilities[i] = p
                # print('probabilities:', ant.probabilities)
                rand = random.random()
                # print('rand', rand)
                for i, probability in enumerate(ant.probabilities):
                    rand -= probability
                    if rand <= 0:
                        selected = i
                        break
                point_i = points[ant.point] # current point
                point_j = points[selected] # next point
                ant.Lk += dist(point_i, point_j)
                ant.point = selected
                ant.visited.append(selected)
                ind = abs((point_j.index-1) - (point_i.index-1)) - 1 + sum(range(n - min((point_j.index-1), (point_i.index-1)), n))
                ant.combinations[ind] = Constants.Q

                ant.probabilities = [0] * n
            d = dist(points[ant.visited[-1]], points[ant.visited[0]])
            ant.Lk += dist(points[ant.visited[-1]], points[ant.visited[0]])
            ind = abs(ant.visited[-1] - ant.visited[0]) - 1 + sum(range(n - min(ant.visited[-1], ant.visited[0]), n))
            ant.combinations[ind] = Constants.Q
            if ant.Lk < minimum:
                minimum = ant.Lk
            # print('Lk', ant.Lk)
            for i in range(len(ant.combinations)):
                ant.combinations[i] = ant.combinations[i]/ant.Lk

        for i in range(int(comb(n, 2))):
            for ant in ants:
                pheromone_sums[i] += ant.combinations[i]
        # if t == 0:
        #     pheromones = [0] * int(comb(n, 2))
        for i in range(len(pheromone_sums)):
            pheromones[i] = pheromones[i] * Constants.Ro + pheromone_sums[i]

        # print('pheromone sums:', pheromone_sums)
        # print('pheromones:', pheromones)
    print("Min:", minimum)
    generate_plot(points)

main()


