/*
The purpose is to count the number of lines in the bad url's. Create a bloom filter with the appropriate
1.5 times the number of lines of space allocated. Then to load the bad url's into the bloom filter
with the use of 3 different hash functions. Then run a mixed url list and test for false positives which 
was found to be a rate of 1.0544% false positive rate. And output the good url's into a separate file. 
*/
#include <iostream>
#include <fstream>
#include "BloomFilter.hpp"

using namespace std;

//Main method 
int main(int argc, char** argv) {
    //variables
    ifstream items1 (argv[1]);
    ifstream items2 (argv[2]);
    ofstream newFile;
    string line;
    int numLines = 0;
    int maxBytes = 0;
    double numFalsePositives = 0.0;
    bool badUrl = false;
    int numBadUrl = 0;
    int numGoodUrl = 0;
    int totalUrl = 0;
    int numSafeUrl = 0;
    //read num of lines
    while (true) {
        getline(items1, line);
        if (items1.eof()) break;
        numBadUrl++;
        numLines++;
    }

    items1.clear();
    items1.seekg(0, std::ios::beg);
    
    maxBytes = (int) (1.5 * numLines);
    BloomFilter* myBloom = new BloomFilter((size_t) maxBytes);

    //Hash into bloom filter
    while(true) {
    	getline(items1, line);
    	if(items1.eof()) break;
    	myBloom->insert(line);
    }
	newFile.open(argv[3]);

    //Test for good or bad filters based on bloom
    while (true) {
    	getline(items2, line);
    	if(items2.eof()) break;
    	totalUrl++;
    	badUrl = myBloom->find(line);
    	if (!badUrl) {
    		newFile << line;
    		numGoodUrl++;
    	}
    }
    newFile.close();

    //calculate false positive rate
    numSafeUrl = totalUrl-numBadUrl;
    numFalsePositives = ((double)(numSafeUrl-numGoodUrl)/(double)(numSafeUrl)) * 100;
    cout << numSafeUrl << " " << numGoodUrl << " " << totalUrl << " " << numBadUrl << endl;
    cout << "The false positive rate is : " << numFalsePositives << "%" << endl;

    //free used space
    delete myBloom;
}
