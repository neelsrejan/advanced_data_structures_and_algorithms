/*
The purpose is to act as the main for creating the Multiway Trie that stores the words and their 
frequencies from the dictionary. The main will then prompt the user to enter either a prefix or a
pattern with a missing char along with the number of competions wished. The program should output 
the desired results.
*/
#include "DictionaryTrie.hpp"
#include "util.hpp"
#include <fstream>

using namespace std;



int main(int argc, char** argv)
{
	string numCompStr, prefix, status = "y";
	int numCompletions;
	//create new trie
	DictionaryTrie* trie = new DictionaryTrie();
	//load dictioary
	ifstream ifs;
  	ifs.open (argv[1]);

	Utils::load_dict(*trie, ifs);
	//prompt user to enter their preferences
	if (argc < 2) {
		cout << "This program needs exactly one argument!" << endl;
		return 1;
	}

	cout << "Reading file: " << argv[1] << endl;

	while (status == "y") {
		cout << "Enter a prefix/pattern to search for:" << endl;
		cin >> prefix;
		cout << "Enter a number of completions:" << endl;
		cin >> numCompStr;

		numCompletions = stoi(numCompStr);
		int foundIdx = prefix.find("_");
		vector<string> comp;
		if (foundIdx == -1) {
			comp = trie->predictCompletions(prefix, numCompletions);
		} else {
			comp = trie->predictUnderscore(prefix, numCompletions);
		}
		 
		for (auto str : comp) {
			cout << str << endl;
		}

		cout << "Continue? (y/n)" << endl;
		cin >> status;
	}
	//free memory
	delete trie;
}
