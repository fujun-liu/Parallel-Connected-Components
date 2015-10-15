
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <ctype.h>
#include <ctime>
#include "GraphDataType.h"
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "UnionFind.h"
#include "MST.h"

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

void LoadGraph(std::string inputfile, MyGraph& Graph, std::unordered_map<long, long>& nodemap){
	std::ifstream ifs(inputfile.c_str());
	std::string line;
	long nodeid = 0;
	if (ifs.is_open()){
		while (std::getline(ifs, line)){
			long fromNodeID, toNodeID;
			if (ParseLine(line, fromNodeID, toNodeID)){
				
				if (nodemap.find(fromNodeID) == nodemap.end()){
					nodemap[fromNodeID] = nodeid++;
				}
				if(Graph.count(fromNodeID) == 0){
		            std::vector<long> edgelist = {toNodeID};
		            Graph[fromNodeID] = edgelist;
		        }else{
		            Graph[fromNodeID].push_back(toNodeID);    
		        }
		        
		        // update to node
		        if (nodemap.find(toNodeID) == nodemap.end()){
		        	nodemap[toNodeID] = nodeid++;
		        }

		        if(Graph.count(toNodeID) == 0){
		            std::vector<long> edgelist = {fromNodeID};
		            Graph[toNodeID] = edgelist;
		        }else{
		            Graph[toNodeID].push_back(fromNodeID);
		        }
			}
		}
		ifs.close();
	}else{
		std::cout << "Input file " << inputfile << " can not be opened! \n";
	}
}


int merge(std::vector<TreeEdges> & allTreeEdges, std::unordered_map<long, long>& nodemap){
	// merge
	UnionFind *uf = new UnionFind(nodemap.size());
	for (TreeEdges& treeedges:allTreeEdges){
		for (auto& p:treeedges){
			uf->Union(nodemap[p.first], nodemap[p.second]);
		}
	}
	const long* labels = uf->GetLabel();
	std::unordered_set<long> compids;
	for (int i = 0; i < nodemap.size(); ++ i){
		if (compids.find(labels[i]) == compids.end()){
			compids.insert(labels[i]);
		}
	}
	return compids.size();
}

int main(){
	std::clock_t start;
	std::string inputfile = "Amazon0302.txt";//"web-googlesmall";
	MyGraph Graph;
	std::unordered_map<long, long> nodemap;

	LoadGraph(inputfile, Graph, nodemap);
	std::cout << "Loading graph takes " << (std::clock() - start)/(double)CLOCKS_PER_SEC << " secs \n";

	MyGraphIterator it = Graph.begin();
	int nParts = 2;
	long  partSize = Graph.size()/nParts;
    
    std::vector<TreeEdges> alledges;
	for (int iPart = 0; iPart < nParts; ++ iPart){
		MST* mst;
		start = std::clock();
		if (iPart == nParts-1){
			mst = new MST(Graph, it, Graph.end());
		}else{
			MyGraphIterator it_old = it;
			std::advance(it, partSize);
			mst = new MST(Graph, it_old, it);
		}
		std::cout << "part " << iPart << " takes " << (std::clock() - start)/(double)CLOCKS_PER_SEC << " secs \n";
		alledges.push_back(mst->GetTreeEdges());
	}

	start = std::clock();
	int ncomps = merge(alledges, nodemap);
	std::cout << "Merging takes " << (std::clock() - start)/(double)CLOCKS_PER_SEC << " secs \n";
	std::cout << "There are " << ncomps << " connected components found\n";
	return 0;
}

