#include "HCNode.hpp"
#include "HCTree.hpp"

int BitInputStream::readBit() {
	if(numBits == 0) {
		fill();
	}

	int bit = (buffer >> (numBits-1)) & 1;
	numBits--;

	return bit;
}

void BitInputStream::fill() {
	buffer = in.get();
	numBits = 8;
}