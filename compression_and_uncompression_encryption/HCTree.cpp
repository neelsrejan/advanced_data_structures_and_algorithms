#include "HCTree.hpp"
#include <string>
using namespace std;

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs) {
	while(priorityFilter.size() > 1){
		//get the two least frequency nodes and pop from queue
		HCNode* leftNode = priorityFilter.top();
		priorityFilter.pop();
		HCNode* rightNode = priorityFilter.top();
		priorityFilter.pop();		
		HCNode* parent = new HCNode((leftNode->count + rightNode->count), rightNode->symbol, leftNode, rightNode, NULL);
		leftNode->p = parent;
		rightNode->p = parent;
		priorityFilter.push(parent);
	}

	root = priorityFilter.top();
}

void HCTree::initNodePtrs(vector<int> freqVec) {
	for (int i = 0; i < freqVec.size(); i++) {
		leaves[i] = new HCNode(freqVec[i],(byte) i, NULL, NULL, NULL);
	}
}

void HCTree::initPriorityLeaves() {
	for (int i = 0; i < leaves.size(); i++) {
		if(leaves[i] != NULL && leaves[i]->count > 0){
			priorityFilter.push(leaves[i]);
		}
	}
}


/** Write to the given ofstream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT
 *  BE USED IN THE FINAL SUBMISSION.
 */

//new
void HCTree::encode(byte symbol, BitOutputStream& out) const {
	int leftNum = 0;
	int rightNum = 1;
	HCNode* currNode = leaves[(int)symbol];
	vector<int> reversed;
	while (currNode->p != NULL) {
		if (currNode == currNode->p->c0) {
			reversed.push_back(leftNum);
		} else {
			reversed.push_back(rightNum);
		}
		currNode = currNode->p;
	}

	for(int i = reversed.size()-1; i >= 0; i--) {
		out.writeBit(reversed[i]);
	}
}

int HCTree::decode(BitInputStream& in) const {

	HCNode* currNode = root;
	while (currNode->c0 != NULL && currNode->c1 != NULL) {
		int read = in.readBit();
		if (read == 0) {
			currNode = currNode->c0;
		} else {
			currNode = currNode->c1;
		}
	}

	return currNode->symbol;
}


void HCTree::readHeader(BitInputStream& iStream) {
	this->bytesCt = 0;
	for(int i = 0; i < 32; i++) {
		this->bytesCt |= (iStream.readBit() << (31-i));
	}

	byte leafCtByte = 0;
	for(int i = 0; i < 8; i++) {
		leafCtByte |= (iStream.readBit() << (7-i));
	}

	int validLeafCt = (int) leafCtByte;
	int* validLeafCtPtr = &validLeafCt;

	//cout << "valid leaf ct: " << validLeafCt << endl;
	readHeaderRecursive(iStream, NULL, validLeafCtPtr);
	
}

void HCTree::readHeaderRecursive(BitInputStream& iStream, HCNode* parentNode, int* leafCt) {
	if (*leafCt <= 0) {
		return;
	}

	int bit = iStream.readBit();
	//cout << "read bit: " << bit << endl;
	HCNode* newNode = NULL;

	if (bit == 0) {
		if (root == NULL) {
			root = new HCNode(0, 0, NULL, NULL, NULL);
			//cout << "created root: " << bit << endl;
			
			readHeaderRecursive(iStream, root, leafCt);
			readHeaderRecursive(iStream, root, leafCt);

			return;
		} else if (parentNode != NULL) {
			//cout << "read bit: " << bit << endl;
			newNode = new HCNode(0, 0, NULL, NULL, NULL);
			
		}
		if (parentNode->c0 != NULL) {
			parentNode->c1 = newNode;
		} else {
			parentNode->c0 = newNode;
		}
		newNode->p = parentNode;
	} else {
		byte symbol = 0;
		for(int i = 0; i < 8; i++) {
			symbol |= iStream.readBit() << (7-i);
		}

		newNode = new HCNode(0, symbol, NULL, NULL, NULL);
		leaves[(int) symbol] = newNode;
		*leafCt = *leafCt - 1;
		//cout << "created leaf: " << (unsigned char) symbol << endl;
		if (parentNode->c0 != NULL) {
			parentNode->c1 = newNode;
		} else {
			parentNode->c0 = newNode;
		}
		newNode->p = parentNode;
		return;
	}

	readHeaderRecursive(iStream, newNode, leafCt);
	readHeaderRecursive(iStream, newNode, leafCt);
}

void HCTree::writeHeader(BitOutputStream& oStream, HCNode* currNode, int numBytes) {
	// Write NumBytes
	vector<int> binaryBytes = intInBits(numBytes);
	//cout<<"printing co: " <<currNode->symbol<<endl;
	for (int i = 0; i < binaryBytes.size(); i++) {
		//cout << i << endl;
		oStream.writeBit(binaryBytes[i]);
	}

	// Write leaf ct
	byte validLeafCt = 0;
	for (int i = 0; i < leaves.size(); i++) {
		if(leaves[i] != NULL && leaves[i]->count > 0){
			validLeafCt++;
		}
	}
	vector<int> binLeafCt = charInBits(validLeafCt);
	//cout<<"printing co: " <<currNode->symbol<<endl;
	for (int i = 0; i < binLeafCt.size(); i++) {
		//cout << i << endl;
		oStream.writeBit(binLeafCt[i]);
	}

	writeHeaderRecursive(oStream, currNode);
	//int* validLeafCtPtr = &validLeafCt;

}

void HCTree::writeHeaderRecursive(BitOutputStream& oStream, HCNode* currNode) {

	if(currNode->c0 == NULL && currNode->c1 == NULL) {
		//cout<<"leaf: 1 " << endl;
		oStream.writeBit(1);
		//cout<<"symbol is: " << currNode->symbol<<endl;
		unsigned char nodeSymbol = (unsigned char) currNode->symbol;
		vector<int> binarySymbol = charInBits(nodeSymbol);
		//cout<<"printing symbol: " <<currNode->symbol<<endl;
		for (int i = 0; i < binarySymbol.size(); i++) {
			oStream.writeBit(binarySymbol[i]);
		}

		//*leafCt = *leafCt - 1;
		return;
	} else {
		oStream.writeBit(0);
	}

	//int createdLeftChild = 0;
	if (currNode->c0 != NULL) {
		//cout<<"left: 0 " << endl;
		//oStream.writeBit(0);
		//cout<<"symbol is: " <<currNode->symbol<<endl;
		writeHeaderRecursive(oStream, currNode->c0);
	} 


	if (currNode->c1 != NULL) {
		//currNode = currNode->c1;
		/*if(currNode->c1->c0 == NULL && currNode->c1->c1 == NULL) {
			oStream.writeBit(1);
		} else {
			oStream.writeBit(0);*/
		writeHeaderRecursive(oStream, currNode->c1);
		//}
	}
}


vector<int> HCTree::charInBits(unsigned char asciiSymbol) {
	vector<int>binaryBitwiseChar;
	byte byteChar = (byte) asciiSymbol;
	bool currBitInSymbol = 0;
	for(int i = 0; i < 8; i++) {
		currBitInSymbol = byteChar >> (7-i) & 1;
		binaryBitwiseChar.push_back((int) currBitInSymbol);
	}
	return binaryBitwiseChar;
}

vector<int> HCTree::intInBits(int asciiSymbol) {
	vector<int>binaryBitwiseChar;
	bool currBitInSymbol = 0;
	for(int i = 0; i < 32; i++) {
		currBitInSymbol = asciiSymbol >> (31-i) & 1;
		binaryBitwiseChar.push_back((int) currBitInSymbol);
	}
	return binaryBitwiseChar;
}

HCTree::~HCTree() {
	delete root;
}








