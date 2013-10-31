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
using namespace std;

class LPCStructure {


public:
	LPCStructure(int constant, int BufDimension);
	virtual ~LPCStructure();
	void AppendLPFC(string appending,unsigned char lcp);
	void Append(string s);
	void SaveBuffer(ofstream out);

	unsigned char *buffer; //buffer to fetch and store the strings
	int *lcpArray; 		//Temporary Array of lcp's
	int lcpindex;
	int C;				//parametric constant to tune the LPFC: no scan more than C characters
	int index;			//Current buffer index
	int NC;				//Number of characters to be scanned to decode the string
	string lastString;	//the last inserted string
};

/* namespace std */
#endif /* LPCSTRUCTURE_H_ */
