#include<iostream>
#include<vector>
#include<cstdint>
#include<atcoder/segtree>
using namespace std;
using namespace atcoder;

// cntはその頂点が最初に現れるときにのみ更新
// LCAはLCA(u,v)= min_{depth_i} RMinQ(cnt_u,cnt_v)

class LCA{
	vector<vector<int>>e;
	segtree<int,[](int a,int b){return a<b?a:b;},[](){return 0;}> seg;
	void dfs(int now,int&cnt){
		seg.set(now,cnt);
		cnt++;
		for(const int&to:e[now])
		if(seg.get(to)!=INT32_MAX)
			dfs(to,cnt);
	}
	public:
	LCA(vector<vector<int>>edge,int root){
		e=move(edge);
		int cnt=0;
		dfs(root,cnt);
		for(int i=0;i<e.size();i++)
			cout<<seg.get(i)<<" \n"[i==e.size()-1];
	}
};
