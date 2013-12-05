#include "LPCStructure.h"
#include <boost/lexical_cast.hpp>
#include "/home/lorenzo/workspaceC/path_decomposed_tries/succinct/util.hpp"
#include "/home/lorenzo/workspaceC/path_decomposed_tries/succinct/test_bp_vector_common.hpp"
#include "/home/lorenzo/workspaceC/path_decomposed_tries/succinct/cartesian_tree.hpp"

uint Lcp2 (string a, string b);

std::string EmitString(int a, std::vector<uint> lcpArray, std::vector<string>);

std::vector<uint> LCP_Builder (std::vector<std::string> stringset);

void TrieVisitSimulationSuccint(LPCStructure *lpfc,std::vector<std::string> stringset, std::ofstream *output, std::vector<uint> lcpArray);

void TrieVisitUtil(LPCStructure *lpfc,std::vector<uint> lcpArray,succinct::cartesian_tree * t,std::vector<std::string> stringset,std::ofstream *output, uint64_t i,uint64_t j);
