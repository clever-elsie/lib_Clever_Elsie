#include<vector>
#include<algorithm>
#ifndef ELSIE_LIS
#define ELSIE_LIS
namespace elsie{
using namespace std;
template<class T>int LIS(const vector<T>&a){
	vector<int> dp(a.size()+1,1ll<<60);
	dp[0]=-(1ll<<60);
	int ans=0;
	for(int i=0;i<a.size();i++){
		int k=lower_bound(begin(dp),end(dp),a[i])-begin(dp);
		if(dp[k-1]<a[i]){
			dp[k]=min(dp[k],a[i]);
			ans=max(k,ans);
		}
	}
	return ans;
}
}
#endif