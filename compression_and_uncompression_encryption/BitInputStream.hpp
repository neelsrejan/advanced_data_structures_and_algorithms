#include <iostream>

class BitInputStream {
private:
	char buffer;
	int numBits;
	std::istream& in;

public:
	BitInputStream(std::istream& input) : in(input) {
		buffer = 0;
		numBits = 0;
	}

	int readBit();

	void fill();
};