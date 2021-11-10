#include "HCTree.hpp"
#include "HCNode.hpp"

using namespace std;

int main(int argc, char** argv) {
	
	ifstream inFile;
	inFile.open(argv[1], ios::binary);
	BitInputStream iStream(inFile);

	HCTree* myTree = new HCTree();

	ofstream outFile;
	outFile.open(argv[2], ios::binary | ios::out);
	if (inFile.peek() == ifstream::traits_type::eof()) {
		inFile.close();
		outFile.close();
		return 0;
	}

	myTree->readHeader(iStream);

	
	while (!inFile.eof()){
		const unsigned char symbol = myTree->decode(iStream);
		if (myTree->bytesCt-- <= 0 || inFile.eof()) break;

		outFile << symbol;
	}	
	outFile.close();
	
}

