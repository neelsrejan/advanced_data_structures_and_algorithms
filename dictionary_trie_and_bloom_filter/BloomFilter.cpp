/*
The purpose is to show the different hash functions used in the bloom filter. 
Also the use of a bloom filter constructor to create a array with a tight defined space
The bloom filter uses an insert method as well as a find method to calculate the collisions. 
*/
#include "BloomFilter.hpp"
#include <iostream>

unsigned int hashStringCRC(const string&   key)    
    {
    unsigned int hashValue = 0;
    for (string::const_iterator i = key.begin();  i != key.end();  i++)
    {
        unsigned int leftShiftedValue = hashValue << CRC_HASH_SHIFT;
        unsigned int rightShiftedValue = hashValue >> (WORD_WIDTH - CRC_HASH_SHIFT);
        hashValue = (leftShiftedValue | rightShiftedValue) ^ (unsigned)*i;
    }
    return hashValue;
    }

unsigned int hashStringWeiss1(
    const string&   key)
    {
    unsigned int hashValue = 0;
    for (string::const_iterator i = key.begin();  i != key.end();  i++)
        hashValue = hashValue ^ (hashValue << WEISS_HASH_SHIFT) ^ (unsigned)*i;
    return hashValue;
    }

unsigned int hashStringWeiss2(
    const string&   key)
    {
    unsigned int hashValue = 0;
    for (string::const_iterator i = key.begin();  i != key.end();  i++)
        hashValue = hashValue * WEISS_HASH_MULTIPLIER + (unsigned)*i;
    return hashValue;
    }


/* Create a new bloom filter with the size in bytes */
BloomFilter::BloomFilter(size_t numBytes)
{
	table = new unsigned char[numBytes];
	numSlots = numBytes * 8;
}

/* Insert an item into the bloom filter */
void BloomFilter::insert(std::string item) 
{
	unsigned int hashVal1 = hashStringCRC(item)%numSlots;
    unsigned int hashVal2 = hashStringWeiss1(item)%numSlots;
    unsigned int hashVal3 = hashStringWeiss2(item)%numSlots;

    vector<int> hashedValues;
    hashedValues.push_back(hashVal1);
    hashedValues.push_back(hashVal2);
    hashedValues.push_back(hashVal3);
    for(int i = 0; i < hashedValues.size(); i++) {
        unsigned int nearestInt = hashedValues[i]/8;
        unsigned int remainder = hashedValues[i]%8;
        table[nearestInt] |= 1 << (7-remainder);
       
    }
}

/* Determine whether an item is in the bloom filter */
bool BloomFilter::find(std::string item) 
{
	unsigned int hashVal1 = hashStringCRC(item)%numSlots;
    unsigned int hashVal2 = hashStringWeiss1(item)%numSlots;
    unsigned int hashVal3 = hashStringWeiss2(item)%numSlots;
    
    vector<int> hashedValues;
    hashedValues.push_back(hashVal1);
    hashedValues.push_back(hashVal2);
    hashedValues.push_back(hashVal3);
	bool found3Times = false;
	int found = 0;
	for (int i = 0; i < hashedValues.size(); i++) {
		unsigned int nearestInt = hashedValues[i]/8;
        unsigned int remainder = hashedValues[i]%8;
        if ((1 & (table[nearestInt] >> (7-remainder))) == 1)  {
        	found++;
        }
	}

	if (found == 3) {
		found3Times = true;
	}
	return found3Times;
}

/* Destrctor for the bloom filter */
BloomFilter::~BloomFilter()
{
	delete [] table;
}
