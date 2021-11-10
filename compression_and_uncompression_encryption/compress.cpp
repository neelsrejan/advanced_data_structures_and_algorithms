#include "HCTree.hpp"
#include "HCNode.hpp"

using namespace std;


int main(int argc, char** argv) {

	//Read chars from text file into a vector that stores the byte 
	//representation of the char.
	ifstream inFile;
	unsigned char nextChar;
	int nextByte;
	inFile.open(argv[1], ios::binary | ios::ate);

	int numBytes = inFile.tellg();
	inFile.clear();
	inFile.seekg(0, ios::beg);

	vector<int> freqVec(256);

	ofstream outFile;
	outFile.open(argv[2], ofstream::out);

	//check for empty file
	if (inFile.peek() == ifstream::traits_type::eof()) {
		inFile.close();
		outFile.close();
		return 0;
	}

	//read chars in input into freqVec
	while((nextByte = inFile.get()) != EOF) {
		nextChar = (byte) nextByte;
		freqVec[(int)nextChar]++;
	}
	//reset pointer to beginning of input file
	inFile.clear();
	inFile.seekg(0, ios::beg);

	//construct a tree by creating pointers to the chars
	//and ordering them by freq then alphabet and building 
	//the huffman tree
	HCTree* myTree = new HCTree();
	myTree->initNodePtrs(freqVec);
	myTree->initPriorityLeaves();
	myTree->build(freqVec);

	BitOutputStream oStream(outFile);
	myTree->writeHeader(oStream, myTree->root, numBytes);
	//oStream.flush();
	
	/*HCNode* tempRoot = new HCNode(41, 'd', NULL, NULL, NULL);
	tempRoot->c0 = new HCNode(20, 'b',NULL,NULL,NULL);
	tempRoot->c1 = new HCNode(21, 'd',NULL,NULL,NULL);
	tempRoot->c0->c0 = new HCNode(10, 'c',NULL,NULL,NULL);
	tempRoot->c0->c1 = new HCNode(10, 'b',NULL,NULL,NULL);
	tempRoot->c1->c0 = new HCNode(10, 'a',NULL,NULL,NULL);
	tempRoot->c1->c1 = new HCNode(11, 'd',NULL,NULL,NULL);
	tempRoot->c1->c1->c0 = new HCNode(1, 'z',NULL,NULL,NULL);
	tempRoot->c1->c1->c1 = new HCNode(10, 'd',NULL,NULL,NULL);
	myTree->writeHeader(oStream, tempRoot);
	oStream.flush();*/
	
	/*
	//create the header for naive submission
	for(int i = 0; i < freqVec.size(); i++) {
		outFile << freqVec[(int) i] << endl;
	}
	*/


	//encode the tree
	while((nextByte = inFile.get()) != EOF) {
		//bytesRead bytesCt
		nextChar = (byte) nextByte;
		myTree->encode(nextByte, oStream);
	}
	oStream.flush();
	
	inFile.close();
	outFile.close();

	delete myTree;
}
