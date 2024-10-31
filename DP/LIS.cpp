#include<vector>
#include<algorithm>
using namespace std;
vector<int> LIS(vector<int>&a){
	int len=0;
	vector<int>dp(a.size()),L(a.size()+1,1ll<<60);
	vector<int>::iterator B=L.begin();
	for(size_t i=0;i<a.size();i++){
		int p=lower_bound(B,B+len+1,a[i])-B;
		dp[i]=p;
		L[p]=a[i];
		if(p>len)
			len++;
	}
	return len+1;
}

int LIS(vector<int>&a){
	vector<int> dp(a.size()+1,1ll<<60);
	dp[0]=0;
	int ans=0;
	for(int i=0;i<n;i++){
		int k=lower_bound(A(dp),a[i])-dp.begin();
		if(dp[k-1]<a[i]){
			dp[k]=min(dp[k],a[i]);
			ans=max(k,ans);
		}
	}
	return ans;
}