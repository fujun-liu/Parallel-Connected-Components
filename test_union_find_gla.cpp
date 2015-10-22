#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <ctype.h>
#include <ctime>
//#define USE_LOCAL_FINALIZER
#ifdef USE_LOCAL_FINALIZER
#include "UnionFindGLA.h"
#else
#include "UnionFindGLA0.h"
#define UnionFindGLA UnionFindGLA0
#endif
bool ParseLine(const std::string& line, long& fromNode, long& toNode){
	// here node id are integer numbers
	int i = 0, i_old, len;
	while (i < line.length() && isspace(line[i])) ++ i; // skip space
	// get first number
	i_old = i;
	while (i < line.length() && line[i] >= '0' && line[i] <= '9') ++ i; // until next non-digit number
	len = i - i_old;
	if (len == 0){
		return false;
	}else{
		fromNode = std::stol(line.substr(i_old, len));
	}

	//next line
	while (i < line.length() && isspace(line[i])) ++ i; // skip space
	i_old = i;
	while (i < line.length() && line[i] >= '0' && line[i] <= '9') ++ i;
	len = i - i_old;
	if (len == 0){
		return false;
	}else{
		toNode = std::stol(line.substr(i_old, len));
	}
	return true;
}

void FindConComps(std::string inputfile, UnionFindGLA& ConComps){
	std::ifstream ifs(inputfile.c_str());
	std::string line;
	if (ifs.is_open()){
		while (std::getline(ifs, line)){
			long fromNode, toNode;
			if (ParseLine(line, fromNode, toNode)){
				ConComps.AddItem(fromNode, toNode);
			}
		}
		ifs.close();
	}else{
		std::cout << "Input file " << inputfile << " can not be opened! \n";
	}

	#ifdef USE_LOCAL_FINALIZER
	// run connected components
	ConComps.FinalizeLocalState();
	#endif
}


int main(){
	std::clock_t start;
	UnionFindGLA ConComps1;
	std::string inputfile1 = "web-googlesmallaa";
	start = std::clock();
	FindConComps(inputfile1, ConComps1);
	std::cout << "It takes " << (std::clock() - start)/(double)CLOCKS_PER_SEC << " secs \n";

	UnionFindGLA ConComps2;
	std::string inputfile2 = "web-googlesmallab";
	start = std::clock();
	FindConComps(inputfile2, ConComps2);
	std::cout << "It takes " << (std::clock() - start)/(double)CLOCKS_PER_SEC << " secs \n";

	start = std::clock();
    ConComps1.AddState(ConComps2);
    std::cout << "It takes AddState " << (std::clock() - start)/(double)CLOCKS_PER_SEC << " secs \n";

	ConComps1.Finalize();
	//
	int ncomps = 0;
	std::set<long> comp;
	while (ConComps1.GetNextResult(comp)){
		++ ncomps;
	}
	std::cout << "There are " << ncomps << " connected components \n";

	return 0;
}