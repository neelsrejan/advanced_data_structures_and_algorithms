# A simple makefile for CSE 100 P2

# We use this in order to feed in the correct version of g++
# (version 4.8) where it's installed under a different name in
# Gradescope. Change the CXX variable assignment at your own risk.
CXX ?= g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS=-g

all: autocomplete benchtrie firewall hashStats

autocomplete: util.o DictionaryTrie.o

benchtrie: util.o DictionaryTrie.o

firewall: BloomFilter.o

hashStats: BloomFilter.o

test: util.o DictionaryTrie.o

DictionaryTrie.o: DictionaryTrie.hpp

BloomFilter.o: BloomFilter.hpp

util.o: util.hpp

clean:
	rm -f test autocomplete benchtrie firewall hashStats *.o core* *~