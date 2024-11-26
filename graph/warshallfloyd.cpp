#include<algorithm>
#include<vector>
using namespace std;

void WF(vector<vector<int64_t>>&e){
	size_t n=e.size();
	for(size_t k=0;k<n;k++)
	for(size_t i=0;i<n;i++)
	for(size_t j=0;j<n;j++)
		e[i][j]=min(e[i][j],e[i][k]+e[k][j]);
}