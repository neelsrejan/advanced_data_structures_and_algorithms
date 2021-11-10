#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <unordered_set>
#include "MovieNode.hpp"
#include "ActorEdge.hpp"

using namespace std;

//Actor class that has a constructor that holds the actor name 
//and a map of the movies that they act in. The class also has the 
//fields for the BFS and Dijkstra's and Kruskal's algorithm
class ActorNode {
public:
	string actorName;
	unordered_set<MovieNode*>* moviesIn;

	int dist;
	bool done;
	ActorEdge* prev;

	int rank; 
	//Constructor
	ActorNode(string actorName){
		this->actorName = actorName;
		moviesIn = new unordered_set<MovieNode*>();
	}
};

#endif