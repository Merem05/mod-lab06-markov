#ifndef INCLUDE_TEXTGEN_H_
#define INCLUDE_TEXTGEN_H_

#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>

using std::deque;
using std::map;
using std::vector;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

typedef deque<string> prefix;
typedef map<prefix, vector<string>> statetab;

const int NPREF = 2;
const int MAXGEN = 1000;

void buildTable(const string& filename, statetab& table, prefix& startPrefix);
string generate(const statetab& table, const prefix& startPrefix, int maxGen);
void saveToFile(const string& filename, const string& text);
void setSeed(unsigned int seed);

#endif
