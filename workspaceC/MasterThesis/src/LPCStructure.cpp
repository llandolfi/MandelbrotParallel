/*
 * LPCStructure.cpp
 *
 *  Created on: 24/ott/2013
 *      Author: lorenzo
 */

#include "LPCStructure.h"
#include "StringFunctions.h"






LPCStructure::LPCStructure(int constant, int BufDimension ) {
	// TODO Auto-generated constructor stub
	C=constant;
	//Parametric buffer size
	buffer = (unsigned char*) malloc(BufDimension);
	lcpArray = (int *)malloc(BufDimension);
	index = 0;
	lcpindex = 0;
	NC = 0;
	lastString = "";
}

LPCStructure::~LPCStructure() {
	// TODO Auto-generated destructor stub
}

void LPCStructure::AppendLPFC(string app, unsigned char lcp){
	//put the string compressed in the buffer

	int ilcp = (int)lcp;
	//if no common characters with last inserted string reset NC
	if (ilcp == 0) NC=0;
	//To use 1 byte instead of 4, put lcp in the buffer instead of ilcp
	if (index == 0){
		buffer[0] = 255;}
	else{
		buffer[index] = ilcp;}
	lcpArray[lcpindex] = buffer[index];
	//cout << lcpArray[lcpindex] << endl;
	lcpindex++;

	//If I have to scan less than C character to decode the string,insert it compressed
	if (NC < C) {
		//cout << (int)buffer[index]<< endl;
		index ++;
		for (int i=0;i<app.size()-ilcp;i++){
			buffer[index+i] = app.c_str()[i+ilcp];
			//cout << (char)buffer[index+i] << endl;
		}
		//update the index and NC
		NC = NC + app.size()-ilcp;
		index = index + app.size()-ilcp;}
	//else insert it uncompressed
	else {
		for (int i=0;i<app.size();i++){
			buffer[index+i] = app.c_str()[i];
			//cout << (char)buffer[index+i] << endl;
		}
		NC = app.size();
		index = index + app.size();
	}
	//update last string
	lastString = app;
	buffer[index] = ')';

}

void LPCStructure::Append(string s){
	unsigned char lcpLast = Lcp2(s,lastString);
	this->AppendLPFC(s,lcpLast);
}

/*void LPCStructure::SaveBuffer(ofstream out){
	out << buffer;
}*/


