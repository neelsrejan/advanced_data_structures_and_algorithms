/*
 * ActorGraph.hpp
 * Author: <Neel Srejan>
 * Date:   <6/3/19>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <fstream>
#include <sstream>
#include <string>
#include "ActorNode.hpp"
#include "MovieNode.hpp"
#include "ActorEdge.hpp"
#include <iostream>
#include <unordered_map>
#include <queue> 

using namespace std;

//Class that overloads the comparator for priority queue's for MST
class ActorEdgePtrComp {
public:
    bool operator()(ActorEdge*& lhs, ActorEdge*& rhs) const {
        return rhs->weight < lhs->weight;
    }
};

//Class that overloads the comparator for priority queue's for Dijkstra
class ActorNodePtrComp {
public:
    bool operator()(pair<ActorNode*, int>& lhs, pair<ActorNode*, int>& rhs) const {
        return rhs.second < lhs.second;
    }
};

//Class that creates the map of actor to actors through movies as the edge.
class ActorGraph {

public:
    unordered_map<string, ActorNode*> actors;
    unordered_map<string, MovieNode*> movies;
    unordered_map<ActorNode*, unordered_set<ActorEdge*>*> actorToActors;
    /**
     * Constuctor of the Actor graph
     */
    //graph constructor
    ActorGraph(void);
    //graph destructor
    ~ActorGraph();
    // Maybe add some more methods here
  
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
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    //Find the actorNode 
    ActorNode* findActorByName(string actorName);
    //Finds the shortest path to an actor in an unweighted graph
    vector<ActorEdge*>* findShortestPathU(ActorNode* start, ActorNode* end);
    //Finds the shortest path to an actor in an weighted graph
    vector<ActorEdge*>* findShortestPathW(ActorNode* start, ActorNode* end);
    //Make MST
    vector<ActorEdge*>* makeSpanningTree();
    //Helper find method to aid with path compression
    ActorNode* find(unordered_map<ActorNode*, ActorNode*>& ds, ActorNode* n);
    //Helper union method to aid with weighted unions 
    void unionMerge(unordered_map<ActorNode*, ActorNode*>& ds, ActorNode* n1, ActorNode* n2);

};


#endif // ACTORGRAPH_HPP
