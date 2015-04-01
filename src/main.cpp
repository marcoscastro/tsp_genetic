#include <iostream>
#include "tsp.h"
using namespace std;

int main()
{
	srand(time(NULL)); // random numbers
	
	// creates the graph1 with parameters: number of vertexes and initial vertex
	Graph * graph1 = new Graph(5, 0);
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
	graph1->addEdge(3, 4, 2);
	graph1->addEdge(4, 3, 2);
	
	// creates the graph2
	Graph * graph2 = new Graph(4, 0);
	// add edges
	graph2->addEdge(0, 1, 2);
	graph2->addEdge(1, 0, 2);
	graph2->addEdge(0, 2, 6);
	graph2->addEdge(2, 0, 6);
	graph2->addEdge(0, 3, 3);
	graph2->addEdge(3, 0, 3);
	graph2->addEdge(1, 3, 7);
	graph2->addEdge(3, 1, 7);
	graph2->addEdge(1, 2, 4);
	graph2->addEdge(2, 1, 4);
	graph2->addEdge(2, 3, 2);
	graph2->addEdge(3, 2, 2);
	
	// creates the graph3
	Graph * graph3 = new Graph(4, 0);
	// add edges
	graph3->addEdge(0, 1, 10);
	graph3->addEdge(1, 0, 10);
	graph3->addEdge(1, 3, 20);
	graph3->addEdge(3, 1, 20);
	graph3->addEdge(0, 2, 15);
	graph3->addEdge(2, 0, 15);
	graph3->addEdge(1, 3, 25);
	graph3->addEdge(3, 1, 25);
	graph3->addEdge(2, 3, 30);
	graph3->addEdge(3, 2, 30);
	graph3->addEdge(1, 2, 35);
	graph3->addEdge(2, 1, 35);
	
	// creates random graph, parameter true is for generate the graph
	Graph * graph4 = new Graph(1000, 0, true);
	graph4->showInfoGraph();
	
	int iterations = 100;
	
	// iterations 100, 1000, 10000, 100000, 1000000
	while(iterations <= 1000000)
	{
		cout << "\nIterations: " << iterations << "\n";
		
		// parameters: the graph, population size, iterations and mutation rate
		// optional parameters: show_population
		Genetic genetic(graph4, 10, iterations, 5, false);

		const clock_t begin_time = clock(); // gets time
		genetic.run(); // runs the genetic algorithm
		cout << "\n\nTime for to run the genetic algorithm: " << float(clock () - begin_time) /  CLOCKS_PER_SEC << " seconds.\n"; // shows time in seconds
		
		iterations = iterations * 10;
	}
	
	return 0;
}
