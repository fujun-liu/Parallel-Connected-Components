#ifndef _UNION_FIND_MAP_H_ 
#define _UNION_FIND_MAP_H_
#include <unordered_map>
// node id are assumed to be non-negative long int 
class UnionFindMap{
	private:
		std::unordered_map<long, long>& parent; 
		std::unordered_map<long, long> sz;
		const long NON_EXISTING_ID = -1;
	public:
		// constructor did nothing
		UnionFindMap(std::unordered_map<long, long>& parent_):parent(parent_){}

		/*void Init(std::unordered_map<long, long>& parent){
			this->parent = parent;
		}*/

		long Find(long i){
			if (parent.find(i) == parent.end()){
				return NON_EXISTING_ID;
			}
			// use path compression here
			while (i != parent[i]){
				parent[i] = parent[parent[i]];
				i = parent[i];
			}
			return i;
		}

		// put merge small tree into higher tree
		// if disjoint, merge and return false
		void Union(long i, long j){
			long ip = Find(i);
			long jp = Find(j);

			if (ip != NON_EXISTING_ID && jp != NON_EXISTING_ID){// both exists
				if (ip != jp){
					if (sz[ip] < sz[jp]){
						parent[ip] = jp; sz[jp] += sz[ip];
					}else{
						parent[jp] = ip; sz[ip] += sz[jp];
					}
				}
			}else if(ip == NON_EXISTING_ID && jp == NON_EXISTING_ID){// both new
				parent[i] = i; sz[i] = 2;
				parent[j] = i; 
			}else if (jp == NON_EXISTING_ID){ // i exists
				parent[j] = ip; sz[ip] ++;
			}else{
				parent[i] = jp; sz[jp] ++;
			}
		}

		
		// 
		void FinalizeRoot(){
			for(std::unordered_map<long, long>::iterator it = parent.begin(); it != parent.end(); ++ it){
				it->second = Find(it->first);
			}
		}

		void Clear(){
			parent.clear();
			sz.clear();
		}

};

#endif