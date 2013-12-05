#include "StringFunctions.h"
#include "LPCStructure.h"
#include <unistd.h>

//Returns the longest common prefix between two strings as a byte
uint Lcp2(string a, string b){
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
	//return lcp[0];
	return ind;

}

//Emits the first lcpArray[a] characters of stringset[a]
std::string EmitString(int a, std::vector<uint> lcpArray, std::vector<std::string> stringset){
	int l = lcpArray[a];
	char * str = (char*)malloc(l+1);
	for (int i=0; i<l; i++){
		str[i] = stringset[a][i];
	}
	return str;
}


//Simulate the visit of a compacted trie given a set of ordered strings and the lcp array
void TrieVisitSimulationSuccint(LPCStructure *lpfc,std::vector<std::string> stringset, std::ofstream *output, std::vector<uint> lcpArray){
	succinct::cartesian_tree t (lcpArray);
	TrieVisitUtil(lpfc,lcpArray,&t,stringset,output,0,lcpArray.size()-1);
}

//method used to simulate the visit DFS of a trie
void TrieVisitUtil(LPCStructure *lpfc,std::vector<uint> lcpArray, succinct::cartesian_tree *t,std::vector<std::string> stringset,std::ofstream *output, uint64_t i,uint64_t j ){
	if(i == j){
		*output << stringset[i] <<"\n";
		cout << stringset[i]<< endl;
		lpfc->Append(stringset[i]);
	}
	else {
		while (lcpArray[i+1] == lcpArray[i] && i+1<j){
			*output << stringset[i]<<"\n";
			cout << stringset[i] << endl;
			lpfc->Append(stringset[i]);
			i = i+1;
		}
		uint64_t newi = t->rmq(i+1,j);
		if (lcpArray[newi] != 0){
			std::string emit = EmitString(newi,lcpArray,stringset);
			*output << emit<<"\n";
			cout << emit << endl;
			lpfc->Append(emit);
		}
		TrieVisitUtil(lpfc,lcpArray,t,stringset,output,i,newi-1);
		TrieVisitUtil(lpfc,lcpArray,t,stringset,output,newi,j);
	}
}





//Builds the LCP-Array from a vector of strings
std::vector<uint> LCP_Builder (std::vector<std::string> stringset){
	std::vector<uint> lcpArray;
	lcpArray.push_back(100000);
	uint lcp;
	std::string first = stringset[0];
	for (int i=1; i<stringset.size(); i++){
		lcp = Lcp2(first,stringset[i]);
		lcpArray.push_back(lcp);
		first = stringset[i];
	}
	return lcpArray;
}

