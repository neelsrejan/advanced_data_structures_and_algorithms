#include <iostream>
#include <ostream>

class BitOutputStream {
private:
	char buffer;
	int numBits;
	ostream& output;

public:
	BitOutputStream(ostream& output) : output(output) {
		buffer = 0;
		numBits = 0;
	}

	void writeBit(int i);

	void flush();
};