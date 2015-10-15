#ifndef _GRAPH_DATA_TYPE_H_
#define _GRAPH_DATA_TYPE_H_

#include <map>
#include <vector>
#include <utility>
#include <unordered_map>

typedef std::map<long, std::vector<long>> MyGraph;
typedef std::map<long, std::vector<long>>::const_iterator MyGraphIterator;
typedef std::vector<std::pair<long, long>> TreeEdges;

#endif