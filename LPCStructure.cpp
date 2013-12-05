/*
 * LPCStructure.cpp
 *
 *  Created on: 24/ott/2013
 *      Author: lorenzo
 */

#include "LPCStructure.h"
#include "StringFunctions.h"






LPCStructure::LPCStructure(uint constant, uint BufDimension ) {
	// TODO Auto-generated constructor stub
	C=constant;
	//Parametric buffer size
	buffer = (unsigned char*) malloc(BufDimension);
	index = 0;
	NC = 0;
	lastString = "";
}

LPCStructure::~LPCStructure() {
	// TODO Auto-generated destructor stub
}

//put the string compressed in the buffer
void LPCStructure::AppendLPFC(string app, uint ilcp){
	positions.push_back(index);
	//if no common characters with last inserted string reset NC
	if (ilcp == 0) NC=0;
	//At the beginning the lcp is set as the highest value possible
	if (index == 0){
		for(int i=0;i<4;i++)
			buffer[i] = 255;
		cout <<"ciao"<<endl;
	}
	else{
		unsigned char * curByte =(unsigned char*) malloc(4);
		for (int i=0; i<4; i++){
			memcpy(curByte,(unsigned char*)&ilcp,4);
			buffer[index+i] = curByte[i];
		}
	}
	index = index+3;

	//If I have to scan less than C character to decode the string,insert it compressed
	if (NC < C) {
		//cout << (int)buffer[index]<< endl;
		index ++;
		for (int i=0;i<app.size()-ilcp;i++){
			buffer[index+i] = app.c_str()[i+ilcp];
		}
		//update the index and NC
		NC = NC + app.size()-ilcp;
		index = index + app.size()-ilcp;}
	//else insert it uncompressed
	else {
		for (int i=0;i<app.size();i++){
			buffer[index+i] = app.c_str()[i];
		}
		NC = app.size();
		index = index + app.size();
	}
	//update last string
	lastString = app;
	buffer[index] = ')';

}

void LPCStructure::Append(string s){
	uint lcpLast = Lcp2(s,lastString);
	this->AppendLPFC(s,lcpLast);
}

/*void LPCStructure::SaveBuffer(ofstream out){
	out << buffer;
}*/


