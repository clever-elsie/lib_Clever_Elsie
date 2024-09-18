#include<vector>

template <class f>
std::vector<std::vector<f>> MP(std::vector<std::vector<f>> &A, int b) {
	auto mul = [&](std::vector<std::vector<f>> &a, std::vector<std::vector<f>> &b) -> std::vector<std::vector<f>> {
		size_t rank = a.size();
		std::vector<std::vector<f>> res(rank, std::vector<f>(rank, 0));
		for (size_t i = 0; i < rank; i++)
			for (size_t j = 0; j < rank; j++)
				for (int k = 0; k < rank; k++)
					res[i][j] += a[i][k] * b[k][j];
		return res;
	};
	size_t rank = A.size();
	std::vector<std::vector<f>> I = A;
	std::vector<std::vector<f>> res(rank, std::vector<f>(rank, 0));
	for (int i = 0; i < rank; i++)
		res[i][i] = 1;
	while (b) {
		if (b & 1) res = mul(res, I);
		I = mul(I, I);
		b >>= 1;
	}
	return res;
}