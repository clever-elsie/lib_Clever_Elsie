#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
using namespace std;

pair<size_t, string> LCS(const string &s,const string &t, int return_lenght_or_string = 0) {
	size_t ss = s.size(), ts = t.size();
	vector<vector<size_t>> dp(ss + 1, vector<int>(ts + 1, 0));
	auto chmax = [&](int A, int B) -> int { return A > B ? A : B; };
	for (size_t i = 1; i <= ss; i++) // O(st*tt)
		for (size_t j = 1; j <= ts; j++)
			if (s[i - 1] != t[j - 1]) dp[i][j] = chmax(dp[i - 1][j], dp[i][j - 1]);
			else dp[i][j] = chmax(dp[i - 1][j - 1] + 1, chmax(dp[i][j - 1], dp[i - 1][j]));

	if (return_lenght_or_string == 0)
		return make_pair(dp[ss][ts], string(""));

	vector<pair<int, int>> index, Stack;
	Stack.push_back(make_pair(ss, ts));
	while (Stack.size() > 0) {
		auto [y, x] = Stack.back();
		int now = dp[y][x];
		Stack.pop_back();
		index.push_back(make_pair(y, x));
		if (now == 0) break;
		while (dp[y - 1][x] == now) y--;
		while (dp[y][x - 1] == now) x--; // O(st+tt)
		Stack.push_back(make_pair(y - 1, x - 1));
	}
	reverse(index.begin(), index.end());
	string res;
	for (auto [i, j] : index)
		if (0 <= i && i < (int)ss)
			res.push_back(s[i]);
	return make_pair(dp[ss][ts], res);
}