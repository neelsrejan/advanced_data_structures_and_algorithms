#ifndef MOVIENODE_HPP
#define MOVIENODE_HPP

#include <unordered_set>

using namespace std;
//for linker errors 
class ActorNode;
//MovieNode class that has a constructor that has movie name
// year of the movie was made and a map of the actors in the movie
class MovieNode {
public:
	string movieName;
	int yearOfRelease;
	unordered_set<ActorNode*>* actorsIn;
	
	//Constructor
	MovieNode(string movieName, int yearOfRelease) {
		this->movieName = movieName;
		this->yearOfRelease = yearOfRelease;
		actorsIn = new unordered_set<ActorNode*>();
	}
};

#endif