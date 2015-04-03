#ifndef TSP_H
#define TSP_H

#include <vector>
#include <map>
#include <set>
#include <utility> // pair
#include <time.h> // time
#include <stdlib.h> // srand, rand


// class that represents the graph
class Graph
{
private:
	int V; // number of vertices
	int total_edges; // total of edges
	int initial_vertex; // initial vertex
	std::map<std::pair<int, int>, int> map_edges; // map of the edges
public:
	Graph(int V, int initial_vertex, bool random_graph = false); // constructor
	void addEdge(int v1, int v2, int weight); // adds a edge
	void showGraph(); // shows all the links of the graph
	void generatesGraph(); // generates a random graph
	void showInfoGraph(); // shows info of the graph
	int existsEdge(int src, int dest); // checks if exists a edge
	friend class Genetic; // to access private membres this class
};

typedef std::pair<std::vector<int>, int> my_pair;


// sort vector with pair
struct sort_pred
{
	bool operator()(const my_pair& firstElem, const my_pair& secondElem)
	{
		return firstElem.second < secondElem.second;
	}
};

// class that represents genetic algorithm
class Genetic
{
private:
	Graph* graph; // the graph
	std::vector< my_pair > population; // each element is a pair: vector and total cost
	int size_population; // size of population
	int real_size_population; // real size population
	int generations; // amount of generations
	int mutation_rate; // mutation rate
	bool show_population; // flag to show population
private:
	void initialPopulation(); // generates the initial population
public:
	Genetic(Graph* graph, int amount_population, int generations, int mutation_rate, bool show_population = true); // constructor
	int isValidSolution(std::vector<int>& solution); // checks if a solution is valid
	void showPopulation(); // shows population
	void crossOver(std::vector<int>& parent1, std::vector<int>& parent2); // makes the crossover
	void insertBinarySearch(std::vector<int>& child, int total_cost); // uses binary search to insert
	void run(); // runs genetic algorithm
	int getCostBestSolution(); // returns cost of the best solution
	bool existsChromosome(const std::vector<int> & v); // checks if exists the chromosome
};

#endif
