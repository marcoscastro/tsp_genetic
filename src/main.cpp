#include <iostream>
#include "tsp.h"
using namespace std;

int main()
{
	Graph * graph1 = new Graph(5); // creates the graph
	
	// add edges
	graph1->addEdge(0, 1, 1);
	graph1->addEdge(1, 0, 1);
	graph1->addEdge(0, 2, 3);
	graph1->addEdge(2, 0, 3);
	graph1->addEdge(0, 3, 4);
	graph1->addEdge(3, 0, 4);
	graph1->addEdge(0, 4, 5);
	graph1->addEdge(4, 0, 5);
	graph1->addEdge(1, 2, 1);
	graph1->addEdge(2, 1, 1);
	graph1->addEdge(1, 3, 4);
	graph1->addEdge(3, 1, 4);
	graph1->addEdge(1, 4, 8);
	graph1->addEdge(4, 1, 8);
	graph1->addEdge(2, 3, 5);
	graph1->addEdge(3, 2, 5);
	graph1->addEdge(2, 4, 1);
	graph1->addEdge(4, 2, 1);
	
	// parameters: the graph, population size and iterations
	Genetic genetic(graph1, 1000, 10);
		
	genetic.run(); // runs the genetic algorithm
	
	return 0;
}
