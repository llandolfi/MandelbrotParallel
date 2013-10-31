#include "LPCStructure.h"

unsigned char Lcp2 (string a, string b);

void TrieVisitSimulation(std::vector<std::string>, std::ofstream *output, int i, int j, int *lpcarray);

std::string EmitString(int a, int * lcpArray, std::vector<string>);

void RangeMinQuery(int i,int j,int *lcpArray,int *Range);
