/*
Header file that has a constuctor for a TrieNode within the class DictionaryTrie.
There are public TrieNode classes to find, insert predictCompletions, and predict underscore.
There are helper functions as private to aid the public methods above. 
*/
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <iostream>
#include <utility>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector>
using namespace std;

//Dictionary Trie Class
class DictionaryTrie
{

public:
  //Class for overloading () operator
	class Compare
	{
	public:
	    bool operator() (pair<string, int> p1, pair<string, int> p2)
	    {
	    	if (p1.second > p2.second)
	    		return false;
	    	else if(p1.second == p2.second)
	    		return (p1.first > p2.first);

	        return true;
	    }
	};

  //Class for TrieNode creation
	class TrieNode 
	{
	public:
		unordered_map<char, TrieNode*>* charMap;
		int frequency;

		TrieNode () {
			frequency = 0;
			charMap = new unordered_map<char, TrieNode*>();
		}
	};

  //Create a new Dictionary
  DictionaryTrie();

  //Inserts a word into the MWT and returns true if added, false if word 
  //already exists or the word is an empty string
  bool insert(std::string word, unsigned int freq);

  //Find and return true if the word is in the trie, false if not.
  bool find(std::string word) const;

  //Predict the completion of a given number of entries based on the prefix of
  //the word passed in. The outputted results will be in order of frequency present
  //If no completions exist, 0 is returned and as many words as can be returned will be 
  //based on the num_completions passed.
  std::vector<std::string>
  predictCompletions(std::string prefix, unsigned int num_completions);

  // Method to predict a missing char in a given pattern eneterd by the user. The method will return the 
//top num_completions of the pattern in decreasing order unless there are none in which case it will return 
//with an empty vector. 
  std::vector<std::string>
  predictUnderscore(std::string pattern, unsigned int num_completions);

  // Destructor 
  ~DictionaryTrie();

private:
  //Root pointer
	TrieNode* root;
  //Helper function to recursively search for the largest frequency word and return using Depth First Search
	void dfs(TrieNode *curr, string soFar, vector<pair<string, int> > & completeList);
  //Helper method to take the pairings of string to frequency and order using a priority queue.
	vector<string> getAutocompletions(vector<pair<string, int>> toBeOrderedList, int numToReturn);
  //Helper function to recursively free allocated space.
  void destroyRecursively(DictionaryTrie::TrieNode* node);
};

#endif // DICTIONARY_TRIE_H
