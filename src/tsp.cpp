#include <iostream>
#include <time.h> // time
#include <stdlib.h> // srand, rand
#include <algorithm> // sort
#include "tsp.h"
using namespace std;


Edge::Edge(int src, int dest, int weight) // constructor of Edge
{
	// assigns the parameters
	this->src = src;
	this->dest = dest;
	this->weight = weight;
}


Graph::Graph(int V) // constructor of Graph
{
	if(V < 1) // checks if number of vertexes is less than 1
	{
		cout << "Error: number of vertexes <= 0\n";
		exit(1);
	}
	
	this->V = V; // assigns the number of vertices
	this->total_edges = 0; // initially the total of edges is 0
}


void Graph::addEdge(int src, int dest, int weight) // add a edge
{
	Edge edge(src, dest, weight); // creates a edge
	edges.push_back(edge); // adds edge in vector of edges
	total_edges++; // increments number total of edges
}


void Graph::showGraph() // shows all connections of the graph
{	
	for(int i = 0; i < total_edges; i++)
		cout << edges[i].src << " linked to vertex " << edges[i].dest << " with weight " << edges[i].weight << endl; 
}


bool Graph::existsEdge(int src, int dest) // checks if exists a edge
{
	for(int i = 0; i < total_edges; i++)
	{
		if(src == edges[i].src && dest == edges[i].dest)
			return true;
	}
	return false;
}


int Graph::getWeightEdge(int src, int dest) // get weight of the edge
{
	for(int i = 0; i < total_edges; i++)
	{
		if(src == edges[i].src && dest == edges[i].dest)
			return edges[i].weight;
	}
	return -1;
}


Genetic::Genetic(Graph* graph, int size_population, int iterations) // constructor of Genetic
{
	if(size_population < 1) // checks if size of population is less than 0
	{
		cout << "Error: size_population <= 0\n";
		exit(1);
	}
	this->graph = graph;
	this->size_population = size_population;
	this->iterations = iterations;
	srand(time(NULL)); // random numbers
}



// checks if is a valid solution, then return total cost of path else return -1
int Genetic::isValidSolution(vector<int>& solution)
{
	set<int> my_set;
	int size_solution = solution.size();
	int total_cost = 0;
	
	// puts in set to eliminate repeated elements
	for(int i = 0; i < size_solution; i++)
		my_set.insert(solution[i]);
	
	// checks if set's size is equal total number of vertices
	if(my_set.size() == (unsigned)graph->V)
	{
		// checks if connections are valid
		for(int i = 0; i < size_solution; i++)
		{
			if(i + 1 < size_solution)
			{
				// checks if exists connection
				if(!graph->existsEdge(solution[i], solution[i+1]))
					return -1;
				else
					total_cost += graph->getWeightEdge(solution[i], solution[i+1]);
			}
			else
			{
				if(!graph->existsEdge(solution[i], solution[0]))
					return -1;
				else
					total_cost += graph->getWeightEdge(solution[i], solution[0]);
				break;
			}
		}
		return total_cost;
	}
	return -1;
}


bool Genetic::existsChromosome(const vector<int> & v)
{
	// checks if exists in the population
	for(vector<pair<vector<int>, int> >::iterator it=population.begin(); it!=population.end(); ++it)
	{
		const vector<int>& vec = (*it).first; // gets the vector
		bool exists_chromosome = true;
		
		for(int i = 0; i < graph->V; i++)
		{
			if(vec[i] != v[i])
			{
				exists_chromosome = false;
				break;
			}
		}
		
		if(exists_chromosome)
			return true;
	}
	return false;
}


void Genetic::initialPopulation() // generates the initial population
{
	vector<int> parent;
	
	for(int i = 0; i < graph->V; i++)
		parent.push_back(i);
		
	int total_cost = isValidSolution(parent);
	if(total_cost != -1)
		population.push_back(make_pair(parent, total_cost));
		
	for(int i = 0; i < size_population; i++)
		crossOver(parent);
		
	size_population = population.size();
		
	// removing repeat elements
	if(size_population == 0)
	{
		cout << "Empty population ;( Try again runs the algorithm...\n";
		exit(1);
	}
}


void Genetic::showPopulation()
{
	cout << "\nShowing population...\n\n";
	for(vector<pair<vector<int>, int> >::iterator it=population.begin(); it!=population.end(); ++it)
	{
		const vector<int>& vec = (*it).first; // gets the vector
		int size_vector = vec.size();
		
		for(int i = 0; i < size_vector; i++)
			cout << vec[i] << " ";
		cout << " | Cost: " << (*it).second << "\n";
	}
}


void Genetic::crossOver(vector<int>& parent)
{	
	// generates points
	int point1 = rand() % (graph->V - 1) + 1; // rand 1 until (graph->V - 1)
	int point2 = rand() % (graph->V - point1) + point1;
	
	if(point1 == point2)
	{
		if(point1 - 1 > 1)
			point1--;
		else if(point2 + 1 < graph->V)
			point2++;
		else
		{
			// point1 or point2 ?? random...
			int point = rand() % 10 + 1; // number in the range 1 to 10
			
			if(point <= 5)
				point1--;
			else
				point2++;
		}
	}
	
	vector<int> child;
	
	// generates child - inverting subsequence of the vertexes
	
	for(int i = 0; i < point1; i++)
		child.push_back(parent[i]);
		
	for(int i = point2; i >= point1; i--)
		child.push_back(parent[i]);
	
	for(int i = point2 + 1; i < graph->V; i++)
		child.push_back(parent[i]);
	
	int total_cost = isValidSolution(child);
	
	// checks if is a valid solution and not exists in the population
	if(total_cost != -1 && !existsChromosome(child))
	{
		population.push_back(make_pair(child, total_cost)); // add in population
		sort(population.begin(), population.end(), sort_pred()); // sort population
	}
}

void Genetic::run()
{
	initialPopulation(); // gets initial population

	for(int i = 0; i < iterations; i++)
	{
		/* selects two parents (if exists) who will participate 
			of the reproduction process */
		if(size_population >= 2)
		{
			if(size_population == 2)
			{
				// applying crossover in the two parents
				crossOver(population[0].first);
				crossOver(population[1].first);
			}
			else
			{
				int parent1, parent2;
			
				do
				{
					parent1 = rand() % size_population;
					parent2 = rand() % size_population;
				}while(parent1 == parent2);
				
				// applying crossover in the two parents
				crossOver(population[parent1].first);
				crossOver(population[parent2].first);
			}
			
			int diff_population = population.size() - size_population;
			
			if(diff_population == 2)
			{
				// removes the two worst parents
				population.pop_back();
				population.pop_back();
			}
			else if(diff_population == 1)
				population.pop_back(); // removes the worst parent
		} 
		else // population contains only 1 parent
		{
			// applying crossover in the parent
			crossOver(population[0].first);
			
			if(population.size() > (unsigned)size_population)
				population.pop_back(); // removes the worst parent
		}
	}
	
	showPopulation();
		
	cout << "\nBest solution: ";
	const vector<int>& vec = population[0].first;
	for(int i = 0; i < graph->V; i++)
		cout << vec[i] << " ";
	cout << " | Cost: " << population[0].second;
}
