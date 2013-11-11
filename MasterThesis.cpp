//============================================================================
// Name        : MasterThesis.cpp
// Author      : Lorenzo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "LPCStructure.h"
#include "StringFunctions.h"
#include "/home/lorenzo/workspaceC/path_decomposed_tries/succinct/elias_fano.hpp"
using namespace std;



int main() {

	//Input and output file for the stringset
	std::ifstream infile("input");
	std::ofstream ofile("output.txt");
	std::ofstream output("visit.txt");
	std::ofstream out("visitsuccinct.txt");

	//String containing the current line of the file
	std::string line;

	//Structure to compress the stringset
	LPCStructure  *lpfc = new LPCStructure (100,1024);



	//Vector of strings storing the stringset
	std::vector<std::string> stringset2;

	//Append each string to LPFC structure and put that string in the array

	cout << "input strings" << endl;
	int j=0;
	while (std::getline(infile, line))
	{

		std::istringstream iss(line);
		stringset2.push_back(line);
		lpfc->Append(line);
		cout << stringset2.at(j) << endl;
		j++;
	}



	//Test LCP-Builder
	std::vector<uint> lcps = LCP_Builder(stringset2);
	for (int k=0; k<lcps.size(); k++){
		cout << lcps[k] << endl;
	}


	//Test trieVisit succinct
	LPCStructure *frontcoded = new LPCStructure(100,1024);
	TrieVisitSimulationSuccint(frontcoded,stringset2,&out,lcps);

	//Test Elias-Fano
	succinct::elias_fano::elias_fano_builder bvb (1024,22);
	for(int i=0; i<frontcoded->positions.size();i++){
		bvb.push_back(frontcoded->positions[i]);
	}

	succinct::elias_fano *el = new succinct::elias_fano(&bvb,false);
	cout <<el->select(4)<<endl;
	cout << frontcoded->positions[3]<< endl;
	//cout << frontcoded->buffer[17];






	//write the result of LPFC in output file
	int i=0;
	while(frontcoded->buffer[i] != ')'){
		ofile << frontcoded->buffer[i];
		i++;
	}
	ofile.close();
	output.close();


	return 0;
}
