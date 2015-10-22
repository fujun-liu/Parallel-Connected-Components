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
class UnionFindGLA0{
	private:
		std::unordered_map<long, long> compIDLocal, compIDGlobal;
		UnionFindMap *localUF, *globalUF;
		std::unordered_map<long, std::set<long>> compOutput;
		std::unordered_map<long, std::set<long>>::iterator CompIterator;
		bool localFinalized = false;
	public:
		// constructor did nothing
		UnionFindGLA0(){
			localUF = new UnionFindMap(compIDLocal);
			globalUF = new UnionFindMap(compIDGlobal);
			#ifdef DEBUG
			std::cout << "constructor is called \n";
			#endif
		}

		// put merge small tree into higher tree
		// if disjoint, merge and return false
		void AddItem(long i, long j){
			localUF->Union(i, j);
		}
        

		// finalize local state
		void FinalizeLocalState(){
			localUF->FinalizeRoot();

			#ifdef DEBUG
			// count number of components
			std::unordered_map<long, std::set<long>> local_comps;
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
		
		void AddState(UnionFindGLA0& other){
			if (!localFinalized){
				FinalizeLocalState();
				localFinalized = true;
			}

			other.FinalizeLocalState();
			std::unordered_map<long, long>& otherState = other.compIDLocal;
			for(std::unordered_map<long, long>::iterator it = otherState.begin(); it != otherState.end(); ++ it){
				if (compIDLocal.find(it->first) != compIDLocal.end()
						&& compIDLocal[it->first] != it->second){ // merge needed
					globalUF->Union(compIDLocal[it->first], it->second);
				}else{
					compIDLocal[it->first] = it->second;
				}
			}
		}
		
		long GetComponentID(long i){
			long comp_id = compIDLocal[i];
			if (compIDGlobal.find(comp_id) != compIDGlobal.end()){
				comp_id = compIDGlobal[comp_id];
			}
			return comp_id;
		}

		void Finalize(){
			globalUF->FinalizeRoot();
			for(std::unordered_map<long, long>::const_iterator it = compIDLocal.begin(); it != compIDLocal.end(); ++ it){
				long comp_id = GetComponentID(it->first);
				if (compOutput.find(comp_id) != compOutput.end()){
					compOutput[comp_id].insert(it->first);
				}else{
					std::set<long> tmp;
					tmp.insert(it->first);
					compOutput[comp_id] = tmp;
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
    	
    	~UnionFindGLA0(){
    		delete localUF;
    		delete globalUF;
    	}

};

#endif
