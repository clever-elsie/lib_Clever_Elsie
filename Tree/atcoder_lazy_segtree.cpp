#include <atcoder/lazysegtree>
using namespace std;
using namespace atcoder;
#define int long long

template <class S> struct sum_lst { S val; size_t size; sum_lst(S a, size_t b) : val(a), size(b) {} };
template <class S, class F> using add_max_lst = lazy_segtree<S, [](S a, S b) { return (a > b ? a : b); }, []() -> S { return -(1ll << 62); }, F, [](F f, S x) { return x + f; }, [](F f, F g) { return g + f; }, []() -> F { return 0; }>;
template <class S, class F> using add_min_lst = lazy_segtree<S, [](S a, S b) { return (a < b ? a : b); }, []() -> S { return 1ll << 62; }, F, [](F f, S x) { return x + f; }, [](F f, F g) { return g + f; }, []() -> F { return 0; }>;
template <class S, class F> using add_sum_lst = lazy_segtree<sum_lst<S>, [](sum_lst<S> a, sum_lst<S> b) -> sum_lst<S> { return sum_lst<S>(a.val + b.val, a.size + b.size); }, []() -> sum_lst<S> { return sum_lst<S>((S)0, 0ull); }, F, [](F f, sum_lst<S> x) -> sum_lst<S> { return sum_lst<S>(x.val + f * x.size, x.size); }, [](F f, F g) { return g + f; }, []() -> F { return 0; }>;
template <class S, class F> using ch_max_lst = lazy_segtree<S, [](S a, S b) { return (a > b ? a : b); }, []() -> S { return -(1ll << 62); }, F, [](F f, S x) -> S { return (f == -(1ll << 62) ? x : f); }, [](F f, F g) -> F { return (f == -(1ll << 62) ? g : f); }, []() -> F { return -(1ll << 62); }>;
template <class S, class F> using ch_min_lst = lazy_segtree<S, [](S a, S b) { return (a < b ? a : b); }, []() -> S { return 1ll << 62; }, F, [](F f, S x) -> S { return (f == (1ll << 62) ? x : f); }, [](F f, F g) -> F { return (f == (1ll << 62) ? g : f); }, []() -> F { return 1ll << 62; }>;
template <class S, class F> using ch_sum_lst = lazy_segtree<sum_lst<S>, [](sum_lst<S> a, sum_lst<S> b) -> sum_lst<S> { return sum_lst<S>(a.val + b.val, a.size + b.size); }, []() -> sum_lst<S> { return sum_lst<S>((S)0, 0ull); }, F, [](F f, sum_lst<S> x) -> sum_lst<S> { if (f != (1ll << 62)) x.val = f * x.size; return x; }, [](F f, F g) { return (f == (1ll << 62) ? g : f); }, []() -> F { return 1ll << 62; }>;