/*
This is a header file that sets up public methods and private variables to be used.
There is also 3 unique method headers for hash functions from the provided document 
given to the class. As well as initilization of fixed variables used by the hash functions. 
*/
#ifndef BLOOM_FILTER_HPP
#define BLOOM_FILTER_HPP

#include <string>
#include <vector>

using namespace std;

/**
 * The class for bloom filter that provides memory efficient check
 * of whether an item has been inserted before. Small amount of 
 * false positives is possible but there will be no false negatives.
 */
class BloomFilter {

public:

    /* Create a new bloom filter with the size in bytes */
    BloomFilter(size_t numBytes);

    /* Insert an item into the bloom filter */
    void insert(std::string item);

    /* Determine whether an item is in the bloom filter */
    bool find(std::string item);

    /* Destrctor for the bloom filter */
    ~BloomFilter();

private:

    // The char array as the hash table
    unsigned char* table;
    unsigned int numSlots;
    
};
//Used by HashString CRC
static const unsigned int BYTE_WIDTH = 8;

static const unsigned int WORD_WIDTH = sizeof (int) * BYTE_WIDTH;

static const unsigned int CRC_HASH_SHIFT = 5;

//Used by hashStringWeiss1
static const unsigned int WEISS_HASH_SHIFT = 5;

//Used by hashStringWeiss2
static const unsigned int WEISS_HASH_MULTIPLIER = 37;

//Hash function method headers
unsigned int    hashStringCRC(const string& key);

unsigned int    hashStringWeiss1(const string& key);
            
unsigned int    hashStringWeiss2(const string& key);
#endif // BLOOM_FILTER
