/*
This file is to create a multiway trie that inserts, finds, predicts completions
as well as predict_underscore by DFS search. 
*/

#include "DictionaryTrie.hpp"

using namespace std;
/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie()
{
	root = new TrieNode();
}

//Inserts a word into the MWT and returns true if added, false if word 
//already exists or the word is an empty string
bool DictionaryTrie::insert(std::string word, unsigned int freq)
{	
	TrieNode* curr = root;

	if(word.length() == 0) 
	{
		return false;
	}

	for (int i = 0; i < word.length(); i++) 
	{
		char currChar = word[i];
		bool nodeExists = (curr->charMap->find(currChar) != curr->charMap->end());
		// If currChar does not exist in the TrieNode's map...
		if (!nodeExists) 
		{
			pair<char, TrieNode*> toInsert (currChar, new TrieNode());
			curr->charMap->insert(toInsert);
		} 
	
		curr = curr->charMap->at(currChar);

		if (i == word.length()-1) 
		{
			if (nodeExists) 
			{
				return false;
			}

			curr->frequency = freq;
		}
	}

	return true;
}


//Find and return true if the word is in the trie, false if not.
bool DictionaryTrie::find(std::string word) const
{
	TrieNode *curr = root;

	for (int i = 0; i < word.length(); i++) 
	{
		char currChar = word[i];
		bool nodeExists = (curr->charMap->find(currChar) != curr->charMap->end());

		if (!nodeExists) {
			return false;
		}

		curr = curr->charMap->at(currChar);
	}

	return true;
}

//Predict the completion of a given number of entries based on the prefix of
//the word passed in. The outputted results will be in order of frequency present
//If no completions exist, 0 is returned and as many words as can be returned will be 
//based on the num_completions passed.
std::vector<std::string> DictionaryTrie::predictCompletions(std::string prefix, unsigned int num_completions)
{
	TrieNode *curr = root;
	vector<string> output;

	if (!find(prefix)) 
	{
		return output;
	} 
	else 
	{
		for (int i = 0; i < prefix.length(); i++) 
		{
			char currChar = prefix[i];
			curr = curr->charMap->at(currChar);
		}

		vector<pair<string, int> > completionList;

		dfs(curr, prefix, completionList);

		output = getAutocompletions(completionList, num_completions);
	}

	return output;
}

//Helper method to take the pairings of string to frequency and order using a priority queue. 
vector<string> DictionaryTrie::getAutocompletions(vector<pair<string, int>> toBeOrderedList, int numToReturn) 
{
	vector<string> finalList;
	priority_queue<pair<string, int>, vector<pair<string, int> >, DictionaryTrie::Compare> orderedList;

	for (int a = 0; a < toBeOrderedList.size(); a++)
	{
		orderedList.push(toBeOrderedList[a]);
	}

	for (int b = 0; b < numToReturn && !orderedList.empty(); b++) 
	{
		pair<string, int> popped = orderedList.top();
		finalList.push_back(popped.first);

		orderedList.pop();
	}

	return finalList;
}

//Helper function to recursively search for the largest frequency word and return using Depth First Search
void DictionaryTrie::dfs(DictionaryTrie::TrieNode *curr, string soFar, vector<pair<string, int> > & completeList) 
{
	if (curr->frequency > 0) {
		completeList.push_back(make_pair(soFar, curr->frequency));
	}

	for (auto it : *(curr->charMap)) {

		dfs(it.second, soFar + it.first, completeList);
	} 
}



// Method to predict a missing char in a given pattern eneterd by the user. The method will return the 
//top num_completions of the pattern in decreasing order unless there are none in which case it will return 
//with an empty vector. 
std::vector<string> DictionaryTrie::predictUnderscore(std::string pattern, unsigned int num_completions)
{
	TrieNode* curr = root;
	vector<string> output;
	vector<pair<string, int> > completionList;
	vector<pair<string, int> > filteredList;
	string partString = "";
	
	if(to_string(pattern[0]) == "_") 
	{
		dfs(curr, partString, completionList);

		for(int j = 0; j < completionList.size(); j++) 
		{
			pair<string, int> currPair = completionList[j];
			if (currPair.first.length() == pattern.length()) 
			{
				int foundIdx = currPair.first.rfind(pattern.substr(1));
				if (foundIdx == 1) 
				{
					filteredList.push_back(currPair);
				}
			}
		}
		output = getAutocompletions(filteredList, num_completions);

		return output;
	}

	int underscIdx = pattern.find("_");

	if (underscIdx == -1)
		return output;

	for (int i = 0; i < underscIdx; i++) 
	{
		curr = curr->charMap->at(pattern[i]);
	}

	if (underscIdx != pattern.length()-1) 
		{
			partString = pattern.substr(0, underscIdx);
			dfs(curr, partString, completionList);
			for(int j = 0; j < completionList.size(); j++) 
			{
				pair<string, int> currPair = completionList[j];
				if (currPair.first.length() == pattern.length()) 
				{
					int foundIdx = currPair.first.rfind(pattern.substr(underscIdx+1));
					if (foundIdx == underscIdx+1) 
					{
						filteredList.push_back(currPair);
					}
				}
			}
			output = getAutocompletions(filteredList, num_completions);
		}
		else if(underscIdx == pattern.length()-1) 
		{
			partString = pattern.substr(0, pattern.length()-1);
			dfs(curr, partString, completionList);
			for(int j = 0; j < completionList.size(); j++) 
			{
				pair<string, int> currPair = completionList[j];
				if (currPair.first.length() == pattern.length()) 
				{
					filteredList.push_back(currPair);	
				}
			}
			output = getAutocompletions(filteredList, num_completions);
		}

	return output;
}

//Destructor for memory
DictionaryTrie::~DictionaryTrie()
{
  destroyRecursively(root);
}
//Helper function to recursively free allocated space.
void DictionaryTrie::destroyRecursively(TrieNode* node) {
  for (auto it : *(node->charMap)) {
    destroyRecursively(it.second);
  }
  delete node->charMap;
  delete node;
}
