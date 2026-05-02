#ifndef TEXTGEN_H
#define TEXTGEN_H

#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef deque<string> prefix;
typedef map<prefix, vector<string>> statetab;

const int NPREF = 2;
const int MAXGEN = 1000;

void buildTable(const string& filename, statetab& table, prefix& startPrefix);
string generate(const statetab& table, const prefix& startPrefix, int maxGen);
void saveToFile(const string& filename, const string& text);
void setSeed(unsigned int seed);

#endif