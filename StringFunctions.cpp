#include "StringFunctions.h"
#include "LPCStructure.h"
#include <unistd.h>

unsigned char Lcp2(string a, string b){
	uint ind = 0;
	bool con = true;
	while (con){
		if (a.c_str()[ind] == b.c_str()[ind])
			ind = ind + 1;
		else
			con = false;
	}
	unsigned char *lcp = (unsigned char *)malloc(1);
	memcpy(lcp,(unsigned char *)&ind,1);
	return lcp[0];

}

//Emits the first lcpArray[a] characters of stringset[a]
std::string EmitString(int a, int * lcpArray, std::vector<std::string> stringset){
	int l = lcpArray[a];
	char * str = (char*)malloc(l+1);
	for (int i=0; i<l; i++){
		str[i] = stringset[a][i];
	}
	return str;
}

//Simulate the visit of a compacted trie given a set of ordered strings
void TrieVisitSimulation(std::vector<std::string> stringset, std::ofstream *output, int i, int j, int * lcpArray){
	if(i == j){
		cout << stringset[i] << endl;
	}
	else {
		int *mins = (int*)malloc(2*sizeof(int));
		//find the range of the first minima in the stringset
		RangeMinQuery(i,j,lcpArray,mins);
		//emit the first characters of the first minimum shared with its previous string
		int newi = mins[0];
		int newj = mins[1];
		free(mins);
		if (lcpArray[newi] != 0){
			std::string emit = EmitString(newi,lcpArray,stringset);
			cout << emit << endl;}
		TrieVisitSimulation(stringset,output,i,newi-1,lcpArray);
		//Possible to avoid all the recursive calls...
		for (int k=1; k<(newj-newi); k++){
			TrieVisitSimulation(stringset,output,newi+k,newi+k,lcpArray);
		}
		TrieVisitSimulation(stringset,output,newj,j,lcpArray);
	}
}

//Find the range of the first minimum elements of lcpArray between position i and j
void RangeMinQuery(int i, int j,int* lcpArray,int *Range){
	int elated =i+1 ;
	for(int k=i+1;k<=j;k++){
		if (lcpArray[k] < lcpArray[elated]){
			elated = k;}}
	int current = elated+1;
	while(lcpArray[current] == lcpArray[elated] && current <=j){
		current ++;}
	current = current -1;
	Range[0] = elated;
	Range[1] = current;

}
