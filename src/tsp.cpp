#include <iostream>
#include <algorithm> // sort, next_permutation
#include "tsp.h"
using namespace std;


Graph::Graph(int V, int initial_vertex, bool random_graph) // constructor of Graph
{
	if(V < 1) // checks if number of vertexes is less than 1
	{
		cout << "Error: number of vertexes <= 0\n";
		exit(1);
	}
	
	this->V = V; // assigns the number of vertices
	this->initial_vertex = initial_vertex; // assigns initial vertex
	this->total_edges = 0; // initially the total of edges is 0
	
	if(random_graph)
		generatesGraph();
}


void Graph::generatesGraph()
{
	vector<int> vec;
	
	// creates the vector
	for(int i = 0; i < V; i++)
		vec.push_back(i);
	
	// generates a random permutation
	random_shuffle(vec.begin(), vec.end());
	
	initial_vertex = vec[0]; // updates initial vertex
	
	int i, weight;
	for(i = 0; i <= V; i++)
	{
		weight = rand() % V + 1; // random weight in range [1,V]
		
		if(i + 1 < V)
			addEdge(vec[i], vec[i + 1], weight);
		else
		{
			// add last edge
			addEdge(vec[i], vec[0], weight);
			break;
		}
	}
	
	int limit_edges = V * (V - 1); // calculates the limit of edges
	int size_edges = rand() % (2 * limit_edges) + limit_edges;
	
	// add others edges randomly
	for(int i = 0; i < size_edges; i++)
	{
		int src = rand() % V; // random source
		int dest = rand() % V; // random destination
		weight = rand() % V + 1; // random weight in range [1,V]
		if(src != dest)
		{
			addEdge(vec[src], vec[dest], weight);
			addEdge(vec[dest], vec[src], weight);
		}
	}
}


void Graph::showInfoGraph()
{
	cout << "Showing info of graph:\n\n";
	cout << "Number of vertices: " << V;
	cout << "\nNumber of edges: " << map_edges.size() << "\n";
}


void Graph::addEdge(int src, int dest, int weight) // add a edge
{
	map_edges[make_pair(src, dest)] = weight; // adds edge in the map
}


void Graph::showGraph() // shows all connections of the graph
{	
	map<pair<int, int>, int>::iterator it;
	for(it = map_edges.begin(); it != map_edges.end(); ++it)
		cout << it->first.first << " linked to vertex " << it->first.second << " with weight " << it->second << endl;
}


int Graph::existsEdge(int src, int dest) // checks if exists a edge
{
	map<pair<int, int>,int>::iterator it = map_edges.find(make_pair(src, dest));
	
	if(it != map_edges.end())
		return it->second; // returns cost
	return -1;
}


// constructor of Genetic
Genetic::Genetic(Graph* graph, int size_population, int generations, int mutation_rate, bool show_population)
{
	if(size_population < 1) // checks if size of population is less than 1
	{
		cout << "Error: size_population < 1\n";
		exit(1);
	}
	else if(mutation_rate < 0 || mutation_rate > 100) // checks if mutation rate is less than 0
	{
		cout << "Error: mutation_rate must be >= 0 and <= 100\n";
		exit(1);
	}
	this->graph = graph;
	this->size_population = size_population;
	this->real_size_population = 0;
	this->generations = generations;
	this->mutation_rate = mutation_rate;
	this->show_population = show_population;
}



// checks if is a valid solution, then return total cost of path else return -1
int Genetic::isValidSolution(vector<int>& solution)
{
	int total_cost = 0;
	set<int> set_solution;
	
	// checks if not contains elements repeated
	for(int i = 0; i < graph->V; i++)
		set_solution.insert(solution[i]);
	
	if(set_solution.size() != (unsigned)graph->V)
		return -1;

	// checks if connections are valid
	for(int i = 0; i < graph->V; i++)
	{
		if(i + 1 <  graph->V)
		{
			int cost = graph->existsEdge(solution[i], solution[i+1]);
			
			// checks if exists connection
			if(cost == -1)
				return -1;
			else
				total_cost += cost;
		}
		else
		{
			int cost = graph->existsEdge(solution[i], solution[0]);
			
			// checks if exists connection
			if(cost == -1)
				return -1;
			else
				total_cost += cost;
			break;
		}
	}
	return total_cost;
}


bool Genetic::existsChromosome(const vector<int> & v)
{
	// checks if exists in the population
	for(vector<pair<vector<int>, int> >::iterator it=population.begin(); it!=population.end(); ++it)
	{
		const vector<int>& vec = (*it).first; // gets the vector
		if(equal(v.begin(), v.end(), vec.begin())) // compares vectors
			return true;
	}
	return false;
}


void Genetic::initialPopulation() // generates the initial population
{
	vector<int> parent;
	
	// inserts initial vertex in the parent
	parent.push_back(graph->initial_vertex);
	
	// creates the parent
	for(int i = 0; i < graph->V; i++)
	{
		if(i != graph->initial_vertex)
			parent.push_back(i);
	}
		
	int total_cost = isValidSolution(parent);
	
	if(total_cost != -1) // checks if the parent is valid
	{
		population.push_back(make_pair(parent, total_cost)); // inserts in the population
		real_size_population++; // increments real_size_population
	}
	
	// makes random permutations "generations" times
	for(int i = 0; i < generations; i++)
	{
		// generates a random permutation
		random_shuffle(parent.begin() + 1, parent.begin() + (rand() % (graph->V - 1) + 1));
		
		int total_cost = isValidSolution(parent); // checks if solution is valid
		
		// checks if permutation is a valid solution and if not exists
		if(total_cost != -1 && !existsChromosome(parent))
		{
			population.push_back(make_pair(parent, total_cost)); // add in population
			real_size_population++; // increments real_size_population in the unit
		}
		if(real_size_population == size_population) // checks size population
			break;
	}
	
	// checks if real_size_population is 0
	if(real_size_population == 0)
		cout << "\nEmpty initial population ;( Try again runs the algorithm...";
	else
		sort(population.begin(), population.end(), sort_pred()); // sort population
}


void Genetic::showPopulation()
{
	cout << "\nShowing solutions...\n\n";
	for(vector<pair<vector<int>, int> >::iterator it=population.begin(); it!=population.end(); ++it)
	{
		const vector<int>& vec = (*it).first; // gets the vector
		
		for(int i = 0; i < graph->V; i++)
			cout << vec[i] << " ";
		cout << graph->initial_vertex;
		cout << " | Cost: " << (*it).second << "\n\n";
	}
	cout << "\nPopulation size: " << real_size_population << endl;
}


// inserts in the vector using binary search
void Genetic::insertBinarySearch(vector<int>& child, int total_cost)
{
	int imin = 0;
	int imax = real_size_population - 1;
	
	while(imax >= imin)
	{
		int imid = imin + (imax - imin) / 2;
		
		if(total_cost == population[imid].second)
		{
			population.insert(population.begin() + imid, make_pair(child, total_cost));
			return;
		}
		else if(total_cost > population[imid].second)
			imin = imid + 1;
		else
			imax = imid - 1;
	}
	population.insert(population.begin() + imin, make_pair(child, total_cost));
}


/*
	Makes the crossover
	This crossover selects two random points
	These points generates substrings in both parents
	The substring inverted of parent1 is placed in parent2 and
	the substring inverted of parent2 is placed in parent1
	
	Example:
		parent1: 1 2 3 4 5
		parent2: 1 2 4 5 3
		
		substring in parent1: 2 3 4
		substring in parent2: 2 4 5
		
		substring inverted in parent1: 4 3 2
		substring inverted in parent2: 5 4 2
		
		child1: 1 5 4 2 5
		child2: 1 4 3 2 3
		
		Children are invalids: 5 appears 2x in child1 and 3 appears 2x in child2
		Solution: map of genes that checks if genes are not used
*/
void Genetic::crossOver(vector<int>& parent1, vector<int>& parent2)
{
	vector<int> child1, child2;
	
	// map of genes, checks if already are selected
	map<int, int> genes1, genes2;
	
	for(int i = 0; i < graph->V; i++)
	{
		// initially the genes not are used
		genes1[parent1[i]] = 0;
		genes2[parent2[i]] = 0;
	}
	
	// generates random points
	int point1 = rand() % (graph->V - 1) + 1;
	int point2 = rand() % (graph->V - point1) + point1;
	
	// adjusts the points if they are equal
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
	
	// generates childs
	
	// until point1, child1 receives genes of the parent1
	// and child2 receives genes of the parent2
	for(int i = 0; i < point1; i++)
	{
		// adds genes
		child1.push_back(parent1[i]);
		child2.push_back(parent2[i]);
		// marks genes
		genes1[parent1[i]] = 1;
		genes2[parent2[i]] = 1;
	}
	
	// marks remaining genes
	for(int i = point2 + 1; i < graph->V; i++)
	{
		genes1[parent1[i]] = 1;
		genes2[parent2[i]] = 1;
	}
		
	// here is the substring inverted
	// child1 receives genes of the parent2 and
	// child2 receives genes of the parent1
	for(int i = point2; i >= point1; i--)
	{
		if(genes1[parent2[i]] == 0) // if the gene is not used
		{
			child1.push_back(parent2[i]);
			genes1[parent2[i]] = 1; // marks the gene	
		}
		else
		{
			// if the gene already is used, chooses gene that is not used
			for(map<int, int>::iterator it = genes1.begin(); it != genes1.end(); ++it)
			{
				if(it->second == 0) // checks if is not used
				{
					child1.push_back(it->first);
					genes1[it->first] = 1; // marks as used
					break; // left the loop
				}
			}
		}
		
		if(genes2[parent1[i]] == 0) // if the gene is not used
		{
			child2.push_back(parent1[i]);
			genes2[parent1[i]] = 1; // marks the gene
		}
		else
		{
			// if the gene already is used, chooses gene that is not used
			for(map<int, int>::iterator it = genes2.begin(); it != genes2.end(); ++it)
			{
				if(it->second == 0) // checks if is not used
				{
					child2.push_back(it->first);
					genes2[it->first] = 1; // marks as used
					break; // left the loop
				}
			}
		}
	}
	
	// ramaining genes: child1 receives genes of the parent1
	// and child2 receives genes of the parent2
	for(int i = point2 + 1; i < graph->V; i++)
	{
		child1.push_back(parent1[i]);
		child2.push_back(parent2[i]);
	}
		
	// mutation
	int mutation = rand() % 100 + 1; // random number in [1,100]
	if(mutation <= mutation_rate) // checks if the random number <= mutation rate
	{
		// makes a mutation: change of two genes
		
		int index_gene1, index_gene2;
		index_gene1 = rand() % (graph->V - 1) + 1;
		index_gene2 = rand() % (graph->V - 1) + 1;
		
		// makes for child1
		int aux = child1[index_gene1];
		child1[index_gene1] = child1[index_gene2];
		child1[index_gene2] = aux;
		
		// makes for child2
		aux = child2[index_gene1];
		child2[index_gene1] = child2[index_gene2];
		child2[index_gene2] = aux;
	}
	
	int total_cost_child1 = isValidSolution(child1);
	int total_cost_child2 = isValidSolution(child2);
	
	// checks if is a valid solution and not exists in the population
	if(total_cost_child1 != -1 && !existsChromosome(child1))
	{
		// add child in the population
		insertBinarySearch(child1, total_cost_child1); // uses binary search to insert
		real_size_population++; // increments the real_size_population
	}
	
	// checks again...
	if(total_cost_child2 != -1 && !existsChromosome(child2))
	{
		// add child in the population
		insertBinarySearch(child2, total_cost_child2); // uses binary search to insert
		real_size_population++; // increments the real_size_population
	}
}


// runs the genetic algorithm
void Genetic::run()
{
	initialPopulation(); // gets initial population
	
	if(real_size_population == 0)
		return;

	for(int i = 0; i < generations; i++)
	{
		int  old_size_population = real_size_population;
		
		/* selects two parents (if exists) who will participate 
			of the reproduction process */
		if(real_size_population >= 2)
		{	
			if(real_size_population == 2)
			{
				// applying crossover in the parents
				crossOver(population[0].first, population[1].first);
			}
			else
			{
				// real_size_population > 2
				
				int parent1, parent2;
			
				do
				{
					// select two random parents
					parent1 = rand() % real_size_population;
					parent2 = rand() % real_size_population;
				}while(parent1 == parent2);
				
				// applying crossover in the two parents
				crossOver(population[parent1].first, population[parent2].first);
			}
			
			// gets difference to check if the population grew 
			int diff_population = real_size_population - old_size_population;
			
			if(diff_population == 2)
			{
				if(real_size_population > size_population)
				{
					// removes the two worst parents of the population
					population.pop_back();
					population.pop_back();
					
					// decrements the real_size_population in 2 units
					real_size_population -= 2;
				}
			}
			else if(diff_population == 1)
			{
				if(real_size_population > size_population)
				{
					population.pop_back(); // removes the worst parent of the population
					real_size_population--; // decrements the real_size_population in the unit
				}
			}
		} 
		else // population contains only 1 parent
		{
			// applying crossover in the parent
			crossOver(population[0].first, population[0].first);
			
			if(real_size_population > size_population)
			{
				population.pop_back(); // removes the worst parent of the population
				real_size_population--; // decrements the real_size_population in the unit
			}
		}
	}
	
	if(show_population == true)
		showPopulation(); // shows the population
	
	cout << "\nBest solution: ";
	const vector<int>& vec = population[0].first;
	for(int i = 0; i < graph->V; i++)
		cout << vec[i] << " ";
	cout << graph->initial_vertex;
	cout << " | Cost: " << population[0].second;
}


int Genetic::getCostBestSolution()
{
	if(real_size_population > 0)
		return population[0].second;
	return -1;
}
