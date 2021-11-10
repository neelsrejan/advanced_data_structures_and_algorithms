#include "HCNode.hpp"

using namespace std;

bool HCNode::compareSymbol(HCNode* parent) {
	return this->symbol < parent->symbol;
}

HCNode::~HCNode() {
	delete c0;
	delete c1;
}
