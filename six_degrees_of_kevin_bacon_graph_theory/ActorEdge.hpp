#ifndef ACTOREDGE_HPP
#define ACTOREDGE_HPP

#include <unordered_set>
#include "ActorNode.hpp"

using namespace std;

//Class that has a constructor that maps actor to actor
//with a movie as the edge that connects the actuors as well 
//a weight of the edge based on the age of the movie.
class ActorEdge {
public:
	string movieName;
	ActorNode* start;
	ActorNode* end;
	int weight;

	//Constuctor
	ActorEdge(string movieName, ActorNode* start, ActorNode* end, int weight) {
		this->movieName = movieName;
		this->start = start;
		this->end = end;
		this->weight = weight;
	}
};

#endif