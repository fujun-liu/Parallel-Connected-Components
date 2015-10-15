#ifndef _UNION_FIND_H_ 
#define _UNION_FIND_H_ 
class UnionFind{
	private:
		long *parent, *sz, N;
	public:
		UnionFind(long N):N(N){
			parent = new long[N];
			sz = new long[N];
			for (long i = 0; i < N; ++ i){
				parent[i] = i;
				sz[i] = 1;
			}
		}


		long Find(long i){
			while (i != parent[i]){
				parent[i] = parent[parent[i]];
				i = parent[i];
			}
			return i;
		}

		// put merge small tree into higher tree
		// if disjoint, merge and return false
		bool Union(long i, long j){
			long ip = Find(i);
			long jp = Find(j);
			if (ip == jp) return true;

			if (sz[ip] < sz[jp]){
				parent[ip] = jp; sz[jp] += sz[ip];
			}else{
				parent[jp] = ip; sz[ip] += sz[jp];
			}
			return false;
		}

		void Label(){
			for (long i = 0; i < N; ++ i)
				parent[i] = Find(i);
		}

		const long* GetLabel(){
			Label();
			return parent;
		}
		
		~UnionFind(){
			delete []parent;
			delete []sz;
		}
    

};

#endif