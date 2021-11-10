#include "HCNode.hpp"
#include "HCTree.hpp"

void BitOutputStream::writeBit(int i) {
	i = (i << (7-numBits));
	buffer |= i;
	numBits++;

	if(numBits == 8){
		flush();
		numBits = 0;
	}
}

void BitOutputStream::flush() {
	output.put(buffer);
	output.flush();
	numBits = 0;
	buffer = 0;
}
