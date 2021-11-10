#include <string.h>
#include "ActorGraph.hpp"

//Main function that either finds the shortest weighted or unweighted path from actor 
//to actor by movie edges
int main(int agrc, char** argv) {
	//create a new graph
	ActorGraph* graph = new ActorGraph();
	//decide if the graph is weighted or not
	bool whichWeight = strcmp(argv[2], "w") == 0;

	//load input file and extract data
	graph->loadFromFile(argv[1], whichWeight);
	ifstream inFile;
	inFile.open(argv[3], ifstream::in);

	ofstream outFile;
	outFile.open(argv[4], ofstream::out);
	outFile<<"(actor)--[movie#@year]-->(actor)--..." << endl;

	bool have_header = false;
	while(inFile) {
        
        string toSearch;
    
        // get the next line
        if (!getline( inFile, toSearch )) break;

        if (!have_header) {

            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( toSearch );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if(record.size() != 2) {
        	continue;
        }

        //load actor info into actorNode
        ActorNode* firstPerson = graph->findActorByName(record[0]);
		ActorNode* lastPerson = graph->findActorByName(record[1]);
		vector<ActorEdge*>* shortPathPtr = NULL;

		//pick which way to find shortest path in graph if weighted or unweighted desired
		if (whichWeight) {
			shortPathPtr = graph->findShortestPathW(firstPerson, lastPerson);
		} else {
			shortPathPtr = graph->findShortestPathU(firstPerson, lastPerson);
		}
		vector<ActorEdge*> shortPath = *shortPathPtr;

		//output the shortest path to an output text file
		outFile<<"("<< firstPerson->actorName << ")";

		for(int i = shortPath.size()-1; i >= 0; i--) {
			outFile << "--[" << shortPath[i]->movieName << "]-->("<< shortPath[i]->end->actorName<< ")";
		}
		delete shortPathPtr;
		outFile<<endl;
	}

	//close both input and output file
	inFile.close();
	outFile.close();
	delete graph;
}
