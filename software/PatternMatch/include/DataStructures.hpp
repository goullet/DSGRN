#ifndef PATTERNMATCH_DATASTRUCTURES_HPP
#define PATTERNMATCH_DATASTRUCTURES_HPP

#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp> // boost can hash "pair"; note: not guaranteed same hash run-to-run
#include <String>
#include <list>
#include <vector>
#include <set>
// #include "DSGRN.h"

const int DIMENSION = 2;

typedef std::list<std::string> patternlist; 
typedef std::pair<uint64_t,patternlist> keypair;

// // I can use the following custom data type if I provide a hash function for it:

// struct patternElement {
// 	std::string extremum;
// 	std::string intermediate;
// };

// typedef std::list<patternElement> patternlist; 

struct wallStruct {
	std::list<uint64_t> outedges;
	// std::list<uint64_t> inedges;
	// double phasespace [DIMENSION];
	// int outsigns [DIMENSION];
	// int insigns [DIMENSION];
	std::vector<std::set<char>> labels;
	// int var_affected;
};

typedef std::vector<wallStruct> wallgraphvector;

// use Shaun's code to get domain graph

#endif