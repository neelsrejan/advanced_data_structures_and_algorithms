#include <string.h>
#include "ActorGraph.hpp"


//Main function that loads in the input file and creates an output file that contains
//the mapping of movies from one actor to another with the minimal weight path to be taken. 
int main(int agrc, char** argv) {
	//create a new graph
	ActorGraph* graph = new ActorGraph();
	//load data in from file and open and create a header for an output file
	graph->loadFromFile(argv[1], true);
	ofstream outFile;
	outFile.open(argv[2], ofstream::out);
	outFile << "(actor)<--[movie#@year]-->(actor)" << endl;

	//create a MST with the data 
	vector<ActorEdge*>* mstPtr = graph->makeSpanningTree();
	vector<ActorEdge*> mst = *mstPtr;

	int totalWeightSum = 0;
	//for every edge in the least weighted path of the MST from actor to actor output the path and calculate the sum of weights 
	for (ActorEdge* e : mst) {
		outFile << "("<< e->start->actorName << ")" << "<--[" << e->movieName << "]-->("<< e->end->actorName<< ")" << endl;
		totalWeightSum += e->weight;
	}

	//output the data of nodes connected, edges present, and sum of weights that the path uses.
	outFile << "#NODE CONNECTED: " << mst.size() + 1 << endl;
	outFile << "#EDGE CHOSEN: " << mst.size() << endl;
	outFile << "TOTAL EDGE WEIGHTS: " << totalWeightSum << endl;

	//close the file.
	outFile.close();

	delete graph;
	delete mstPtr;
}