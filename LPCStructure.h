/*
 * LPCStructure.h
 *
 *  Created on: 24/ott/2013
 *      Author: lorenzo
 */

#ifndef LPCSTRUCTURE_H_
#define LPCSTRUCTURE_H_
#include "stdlib.h"
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

class LPCStructure {


public:
	LPCStructure(uint constant, uint BufDimension);
	virtual ~LPCStructure();
	void AppendLPFC(string appending,uint lcp);
	void Append(string s);
	void SaveBuffer(ofstream out);

	unsigned char *buffer; //buffer to fetch and store the strings
	uint C;				//parametric constant to tune the LPFC: no scan more than C characters
	uint index;			//Current buffer index
	uint NC;				//Number of characters to be scanned to decode the string
	string lastString;	//the last inserted string
	std::vector<uint> positions;
};

/* namespace std */
#endif /* LPCSTRUCTURE_H_ */
