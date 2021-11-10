#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include <fstream>
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return *lhs < *rhs;
    }
};

/** A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
    vector<HCNode*> leaves;
	//create a vector of HCNode ptrs to all 256 acsii values
	// vector<HCNode*> leafPtrs;
	//create a vecotor of HCNode prts to all ascii values that have a 
	//frequency of more than 0
	vector<HCNode*> filteredLeaves;
	//make a min heap priority queue out of filtered nodes.
	priority_queue <HCNode*, vector<HCNode*>, HCNodePtrComp> priorityFilter;
public:
    HCNode* root;

    int bytesCt;
    explicit HCTree() : root(0) {
        leaves = vector<HCNode*>(256, (HCNode*) 0);
        filteredLeaves = vector<HCNode*>(256, (HCNode*) 0);
        priorityFilter = priority_queue <HCNode*, vector<HCNode*>, HCNodePtrComp>();
    }

    ~HCTree();

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);

    //Post-checkpoint
    //new
    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;


    /** Write to the given ofstream
     *  the sequence of bits (as ASCII) coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT
     *  BE USED IN THE FINAL SUBMISSION.
     */
    //void encode(byte symbol, ofstream& out) const;


    //Post-checkpoint
    //new
    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in) const;

    /** Return the symbol coded in the next sequence of bits (represented as
     *  ASCII text) from the ifstream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
     *  IN THE FINAL SUBMISSION.
     */
    //int decode(ifstream& in) const;

    void initNodePtrs(vector<int> freqVec);

	void initPriorityLeaves();

    void writeHeader(BitOutputStream& oStream, HCNode* currNode, int numBytes);
    void writeHeaderRecursive(BitOutputStream& oStream, HCNode* currNode);

    void readHeader(BitInputStream& iStream);
    void readHeaderRecursive(BitInputStream& iStream, HCNode* parentNode, int* leafCt);

    vector<int> charInBits(unsigned char asciiSymbol);
    vector<int> intInBits(int asciiSymbol);
	//int decodeHelper(ifstream& input, HCNode* currNode) const;

};

#endif // HCTREE_H
