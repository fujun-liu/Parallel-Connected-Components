#ifndef _MST_H_
#define _MST_H_

#include <map>
#include <vector>
#include <utility>
#include <unordered_map>
#include "UnionFind.h"
#include "GraphDataType.h"

class MST{
private:
	MyGraph & Graph;
	MyGraphIterator it_beg, it_end;

	// minimum spanning tree
	TreeEdges trees;
	std::unordered_map<long, long> nodemap;
public:
	MST(MyGraph & Graph, MyGraphIterator it_beg, MyGraphIterator it_end):Graph(Graph), it_beg(it_beg), it_end(it_end){
		// convert node vals to 0 to n-1
		long nodeid = 0; 
		for (MyGraphIterator it = it_beg; it != it_end; ++ it){
			if (nodemap.find(it->first) == nodemap.end()){
				nodemap[it->first] = nodeid++;
			}
			for (long neighbor:it->second){
				if (nodemap.find(neighbor) == nodemap.end()){
					nodemap[neighbor] = nodeid++;
				}
			}
		}

		//std::cout << "There are " << nodemap.size() << " nodes \n";
		// run minimum spanning tree
		UnionFind *uf = new UnionFind(nodemap.size());
		bool stop = false;
		for (MyGraphIterator it = it_beg; !stop && it != it_end; ++ it){
			long from = it->first;

			for (long to:it->second){
				// if not connected, merge these two components
				if (!uf->Union(nodemap[from], nodemap[to])){
					trees.push_back(std::make_pair(from, to));
					if (trees.size() == nodemap.size()-1){
						stop = true;
						break;
					}
				}
			}
		}
	}

	const TreeEdges GetTreeEdges(){
		return trees;
	}

	
};

#endif