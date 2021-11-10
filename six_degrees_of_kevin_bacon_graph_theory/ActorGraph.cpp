/*
 * ActorGraph.cpp
 * Author: <Neel Srejan>
 * Date:   <6/3/19>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include "ActorGraph.hpp"

using namespace std;

/**
 * Constructor of the Actor graph
 */ 
ActorGraph::ActorGraph(void) {

}

/**
 * Destructor
*/
ActorGraph::~ActorGraph(void) {
    for (auto it : actorToActors ) {
            auto edgeSet = it.second;
            for (ActorEdge* edge : *edgeSet) {
                delete edge;
            }
            delete edgeSet;
    }

    for (auto it : actors) {
        delete it.second->moviesIn;
        delete it.second;
    }

    for (auto it : movies) {
        delete it.second->actorsIn;
        delete it.second;
    }
}

/** You can modify this method definition as you wish
 *
 * Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename - input filename
 * use_weighted_edges - if true, compute edge weights as 1 + (2019 - movie_year),
 *                      otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {

    // Initialize the file stream
    ifstream infile(in_filename);
    bool have_header = false;
  	int edgeCount = 0;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {

            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }
        string actorName(record[0]);
        //string movieTitle(record[1]);
        int movieYear = stoi(record[2]);
        //concat movie and year together 
        string movieWithYear = record[1] + "#@" + record[2];

        ActorNode* currActor = NULL;
        MovieNode* currMovie = NULL;

        //create actorNode for new actors 
        if (actors.find(actorName) == actors.end()) {
            currActor = new ActorNode(actorName);
            actors[actorName] = currActor;
        } else {
            currActor = actors[actorName];
        }
        //create movieNodes for new Movies
        if (movies.find(movieWithYear) == movies.end()) {
            currMovie = new MovieNode(movieWithYear, movieYear);
            movies[movieWithYear] = currMovie;
        } else {
            currMovie = movies[movieWithYear];
        }

        //add movies to an actors moviesIn field
        currActor->moviesIn->insert(currMovie);
        //add actors to a movies actorsIn field
        currMovie->actorsIn->insert(currActor);
    }

    //three nested for loops that go through the actors and movies map and create 
    //a actor edge which maps an actor to an actor through a common movie.
    //There can be many movie edges between the same actor pairs.
    for (auto it = actors.begin(); it != actors.end(); ++it) {
        ActorNode* currActor = it->second;

        for (auto it2 = currActor->moviesIn->begin(); it2 != currActor->moviesIn->end(); ++it2) {
            MovieNode* currMov = *it2;

	        // final submission
	        int movieWeight = 1;
	        if(use_weighted_edges) {
	        	movieWeight = 1 + (2019-currMov->yearOfRelease);
	        } else {
	        	movieWeight = 1;
	        }

            if (actorToActors.find(currActor) == actorToActors.end()) {
                actorToActors[currActor] = new unordered_set<ActorEdge*>();
            }

            for (auto it3 = currMov->actorsIn->begin(); it3 != currMov->actorsIn->end(); ++it3) {
                ActorNode* otherActor = *it3;

                if (otherActor == currActor) {
                    continue;
                }
                actorToActors[currActor]->insert(new ActorEdge(currMov->movieName, currActor, otherActor, movieWeight));
                edgeCount++;
            }
        }    
    }
    
    //Close File
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;

}

//Find actor by name in actor map
ActorNode* ActorGraph::findActorByName(string actorName) {
    return actors[actorName];
}

//Function that finds the shortest path from actor to actor by unweighted movie edges
vector<ActorEdge*>* ActorGraph::findShortestPathU(ActorNode* start, ActorNode* end) {
    // clear previous search state
    for (auto it = actors.begin(); it != actors.end(); ++it) {
        ActorNode* actor = it->second;
        actor->dist = std::numeric_limits<int>::max();
        actor->prev = NULL;
    }

    // setup queue
    queue<ActorNode*>* sq = new queue<ActorNode*>();

    start->dist = 0;
    sq->push(start);

    // while still elements in queue
    while(!sq->empty()) {
        // pops from front of queue
        ActorNode* curr = sq->front();
        sq->pop();

        auto neighbors = actorToActors[curr];

        for (ActorEdge* neighborEdge : *neighbors) {
            // get neighbor
            ActorNode* nb = neighborEdge->end;

            // if dist to nb is INF through BFS
            if (nb->dist == std::numeric_limits<int>::max()) {
                nb->dist = curr->dist + 1;
                nb->prev = neighborEdge;

                // enqueue neighbor node
                sq->push(nb);
            }
        }
    }

    //vector that holds the shortest edges that make the shortest path
    vector<ActorEdge*>* shortPath = new vector<ActorEdge*>();
    ActorEdge* pathEdge = end->prev;
    while(pathEdge != NULL) {
        shortPath->push_back(pathEdge); 
        pathEdge = pathEdge->start->prev;
    }

    delete sq;

    return shortPath;
}

//Function that finds the shortest path from actor to actor by weighted movie edges 
vector<ActorEdge*>* ActorGraph::findShortestPathW(ActorNode* start, ActorNode* end) {
    //create a priority queue of pairs of verticies with their distance field through the path.
    priority_queue<pair<ActorNode*, int>, vector<pair<ActorNode*, int>>, ActorNodePtrComp>* pq = 
        new priority_queue<pair<ActorNode*, int>, vector<pair<ActorNode*, int>>, ActorNodePtrComp>();


    // clear previous search state
    for (auto it = actors.begin(); it != actors.end(); ++it) {
        ActorNode* actor = it->second;
        actor->dist = std::numeric_limits<int>::max();
        actor->prev = NULL;
        actor->done = false;
    }

    // enqueue start node
    start->dist = 0;
    pq->push(make_pair(start, 0));

    // while nodes still in pq
    while (!pq->empty()) {

        // remove from top of pq
        auto currPair = pq->top();
        pq->pop();

        ActorNode* curr = currPair.first;
        int distThruCurr = currPair.second;

        if (!curr->done) {
            curr->done = true;

            auto neighbors = actorToActors[curr];

            for (ActorEdge* neighborEdge : *neighbors) {
                // get neighbor
                ActorNode* nb = neighborEdge->end;

                // potential dist to nb thru this edge of curr
                int c = curr->dist + neighborEdge->weight;

                // if new potential shortest path to nb is
                // found, set fields and enqueue
                if (c < nb->dist) {
                    nb->prev = neighborEdge;
                    nb->dist = c;

                    // enqueue neighbor node
                    pq->push(make_pair(nb, c));
                }
            }
        }
    }

    //vector that holds the shortest edges that make the shortest path
    vector<ActorEdge*>* shortPath = new vector<ActorEdge*>();
    ActorEdge* pathEdge = end->prev;
    while(pathEdge != NULL) {
        shortPath->push_back(pathEdge); 
        pathEdge = pathEdge->start->prev;
    }

    delete pq;

    return shortPath;
}

//function that creates the MST
vector<ActorEdge*>* ActorGraph::makeSpanningTree() {
    // priority queue to store all ActorEdge*s
    priority_queue<ActorEdge*, vector<ActorEdge*>, ActorEdgePtrComp>* pq = 
        new priority_queue<ActorEdge*, vector<ActorEdge*>, ActorEdgePtrComp>();

    // initialize our disjoint set
    unordered_map<ActorNode*, ActorNode*>* dsPtr = new unordered_map<ActorNode*, ActorNode*>();
    unordered_map<ActorNode*, ActorNode*> ds = *dsPtr;

    vector<ActorEdge*>* mst = new vector<ActorEdge*>();

    for (auto pair : actors) {
        pair.second->rank = 1;
        ds[pair.second] = NULL;
    }

    // enqueue all actor edges
    for (auto it : actorToActors ) {
        auto edgeSet = it.second;
        for (ActorEdge* edge : *edgeSet) {
            pq->push(edge);
        }
    }
    //while the priority queue is not empty keep popping from the min to max weighted
    //queue and use the find method to see if the actors have the same sentinel, if so 
    //then ignore the edge and continue on and connect nodes through merges if find 
    //gives different sentinels.
    while(!pq->empty()) {
        ActorEdge*minWeightEdge = pq->top();
        pq->pop();

        ActorNode* s1 = find(ds, minWeightEdge->start);
        ActorNode* s2 = find(ds, minWeightEdge->end);
        if (s1 != s2) {
            unionMerge(ds, s1, s2);
            mst->push_back(minWeightEdge);
        }
    }

    delete dsPtr;
    delete pq;

    return mst;
}


//Helper function to find the sentinel of the actor and compress by moving the actor to the respective 
//sentinel as well as others not connected to the sentinel while iterating towards the sentinel
ActorNode* ActorGraph::find(unordered_map<ActorNode*, ActorNode*>& ds, ActorNode* n) {
    ActorNode* curr = n;
    vector<ActorNode*> alongPath;

    // find sentinel iteratively
    // such that at end of loop, curr = sentinel
    while(ds[curr] != NULL) {
        alongPath.push_back(curr);
        curr = ds[curr];
    }

    // path compress
    for (ActorNode* inPath : alongPath) {
        ds[inPath] = curr;
    }

    return curr;
}

//Helper function to merge sentinel of one actor with the sentinel of another actor by a weighted union.
//Keeps track of the size of each union by updating the rank when a union takes place.
void ActorGraph::unionMerge(unordered_map<ActorNode*, ActorNode*>& ds, ActorNode* s1, ActorNode* s2) {
    //check which sentinel has the higher rank and merge smaller set with the larger one.
    if (s1->rank >= s2->rank) {
        ds[s2] = s1;
        s1->rank += s2->rank;
    } else {
        ds[s1] = s2;
        s2->rank += s1->rank;
    }
}

