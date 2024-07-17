#include<vector>
#include<algorithm>
int LIS(std::vector<int>&a){
	int len=0;
	std::vector<int>dp(a.size()),L(a.size()+1,1ll<<60);
	std::vector<int>::iterator B=L.begin();
	for(size_t i=0;i<a.size();i++){
		int p=lower_bound(B,B+len+1,a[i])-B;
		dp[i]=p;
		L[p]=a[i];
		if(p>len)
			len++;
	}
	return len+1;
}