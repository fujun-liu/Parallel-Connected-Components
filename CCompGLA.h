#ifndef _UNION_FIND_GLA_H_
#define _UNION_FIND_GLA_H_
#include <unordered_map>
#include <set>
#include "UnionFindMap.h"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif
// node id are assumed to be non-negative long int 
class CCompGLA{
	private:
		//std::unordered_map<long, long> compIDLocal, compIDGlobal;
		UnionFindMap localUF, globalUF;
		std::unordered_map<long, std::set<long>> compOutput;
		std::unordered_map<long, std::set<long>>::iterator CompIterator;
		bool localFinalized = false;
	public:
		// constructor did nothing
		CCompGLA(){}

		// put merge small tree into higher tree
		// if disjoint, merge and return false
		void AddItem(long i, long j){
			localUF.Union(i, j);
		}
        

		// finalize local state
		void FinalizeLocalState(){
			if (!localFinalized){
				localUF.FinalizeRoot();
				localFinalized = true;
			}

			#ifdef DEBUG
			// count number of components
			std::unordered_map<long, std::set<long>> local_comps;
			std::unordered_map<long, long>& compIDLocal = localUF.GetUF();

			for (auto const& entry:compIDLocal){
				long comp_id = entry.second;
				if (local_comps.find(comp_id) != local_comps.end()){
					local_comps[comp_id].insert(entry.first);
				}else{
					std::set<long> tmp; tmp.insert(entry.first);
					local_comps[comp_id] = tmp;
				}
			}

			std::cout << "There are " << local_comps.size() << " components in the partition \n";
			#endif
		}
		

		void AddState(CCompGLA& other){
			FinalizeLocalState();
			other.FinalizeLocalState();
			std::unordered_map<long, long>& compIDLocal = localUF.GetUF();
			std::unordered_map<long, long>& otherState = other.localUF.GetUF();

			for(auto const& entry:otherState){
				if (compIDLocal.find(entry.first) != compIDLocal.end()
						&& compIDLocal[entry.first] != entry.second){ // merge needed
					globalUF.Union(compIDLocal[entry.first], entry.second);
				}else{
					compIDLocal[entry.first] = entry.second;
				}
			}

			// merge local and global
			globalUF.FinalizeRoot();
			std::unordered_map<long, long>& compIDGlobal = globalUF.GetUF();
			for (auto& p:compIDLocal){
				if (compIDGlobal.find(p.second) != compIDGlobal.end()){
					p.second = compIDGlobal[p.second];
				}
			}
			globalUF.Clear();
		}

		void Finalize(){
			std::unordered_map<long, long>& compIDLocal = localUF.GetUF();
			for(auto const& entry:compIDLocal){
				if (compOutput.find(entry.second) != compOutput.end()){
					compOutput[entry.second].insert(entry.first);
				}else{
					std::set<long> tmp;
					tmp.insert(entry.first);
					compOutput[entry.second] = tmp;
				}
			}

			CompIterator = compOutput.begin();
		}
		
		bool GetNextResult(std::set<long> &comp){
			if (CompIterator != compOutput.end()){
				comp.insert(CompIterator->second.begin(), CompIterator->second.end());
				++ CompIterator;
				return true;
			}else{
				return false;
			}
		}
    	
    	~CCompGLA(){}

};

#endif
