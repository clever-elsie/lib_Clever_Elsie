#include<vector>
#include<algorithm>
using namespace std;

template<class T>
int LIS(const vector<T>&a){
	vector<int> dp(a.size()+1,1ll<<60);
	dp[0]=0;
	int ans=0;
	for(int i=0;i<n;i++){
		int k=lower_bound(begin(dp),end(dp),a[i])-begin(dp);
		if(dp[k-1]<a[i]){
			dp[k]=min(dp[k],a[i]);
			ans=max(k,ans);
		}
	}
	return ans;
}