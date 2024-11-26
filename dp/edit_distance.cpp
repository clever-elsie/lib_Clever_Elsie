#include<string>
#include<vector>
#include<cstddef>
using namespace std;

int edit_dist(string&s,string&t){
	vector dp(s.size()+1,vector<size_t>(t.size()+1,1ll<<60));
	dp[0][0]=0;
	for(size_t i=0;i<dp.size();i++){
		for(size_t j=0;j<dp[i].size();j++){
			if(i>0) dp[i][j]=min(dp[i-1][j]+1,dp[i][j]);
			if(j>0) dp[i][j]=min(dp[i][j-1]+1,dp[i][j]);
			if(i>0&&j>0) dp[i][j]=min(dp[i-1][j-1]+(s[i-1]!=t[j-1]),dp[i][j]);
		}
	}
	return dp.back().back();
}