#ifndef TSP_H
#define TSP_H

#include <vector>
#include <set>
#include <utility> // pair
#include <time.h> // time
#include <stdlib.h> // srand, rand


// class that represents the edge of graph
class Edge
{
private:
	int src, dest, weight; // source, destination and weight
public:
	Edge(int src, int dest, int weight); // constructor
	friend class Graph; // to access private members this class
};


// class that represents the graph
class Graph
{
private:
	int V; // number of vertices
	int total_edges; // total of edges
	int initial_vertex; // initial vertex
	std::vector<Edge> edges; // vector of edges
public:
	Graph(int V, int initial_vertex); // constructor
	void addEdge(int v1, int v2, int weight); // adds a edge
	void showGraph(); // shows all the links of the graph
	bool existsEdge(int src, int dest); // checks if exists a edge
	int getWeightEdge(int src, int dest); // gets weight of the edge
	friend class Genetic; // to access private membres this class
};

typedef std::pair<std::vector<int>, int> my_pair;

/*
// old implementation: to insert an element, the vector is ordered
struct sort_pred 
{
	bool operator()(const my_pair& firstElem, const my_pair& secondElem)
	{
		return firstElem.second < secondElem.second;
	}
};
*/

// class that represents genetic algorithm
class Genetic
{
private:
	Graph* graph; // the graph
	std::vector< my_pair > population; // each element is a pair: vector and total cost
	int size_population; // size of population
	int real_size_population; // real size population
	int iterations; // amount of iterations
	int mutation_rate; // mutation rate
private:
	void initialPopulation(); // generates the initial population
public:
	Genetic(Graph* graph, int amount_population, int iterations, int mutation_rate); // constructor
	int isValidSolution(std::vector<int>& solution); // checks if a solution is valid
	void showPopulation(); // shows population
	void crossOver(std::vector<int>& parent); // makes the crossover
	void insertBinarySearch(std::vector<int>& child, int total_cost); // uses binary search to insert
	void run(); // runs genetic algorithm
	bool existsChromosome(const std::vector<int> & v); // checks if exists the chromosome
};

#endif
