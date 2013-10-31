//============================================================================
// Name        : MasterThesis.cpp
// Author      : Lorenzo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "LPCStructure.h"
#include "StringFunctions.h"
using namespace std;

int main() {

	//Input and output file for the stringset
	std::ifstream infile("input");
	std::ofstream ofile("output.txt");
	std::ofstream output("visit.txt");

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
		cout << stringset2.at(j) << " " << lpfc->lcpArray[j] << endl;
		j++;
	}



	//Test the function to emit the initial chars of a string
	std::string m = EmitString(1,lpfc->lcpArray,stringset2);

	//Test RangeMinQuery
	int * R = (int*)malloc(2*sizeof(int));
	RangeMinQuery(0,7,lpfc->lcpArray,R);


	//Test TrieVisit
	TrieVisitSimulation(stringset2, &output, 0, 12,lpfc->lcpArray);

	//write the result of LPFC in output file
	int i=0;
	while(lpfc->buffer[i] != ')'){
		ofile << lpfc->buffer[i];
		i++;
	}
	ofile.close();


	return 0;
}
