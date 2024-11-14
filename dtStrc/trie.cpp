#include <algorithm>
#include <new>
#include <string>
#include <vector>

class trie {
	struct node {
		int *cnt;
		node **next;
		node(int &size) {
			cnt = new int[size];
			next = new node *[size];
			for (int i = 0; i < size; i++) {
				cnt[i] = 0;
				next[i] = nullptr;
			}
		}
	} *tree;
	int count_prefix_internal(node *here) {
		int res = 0;
		for (int i = 0; i < char_size; i++)
			if (here->cnt[i] > 1)
				res += (here->cnt[i]) * (here->cnt[i] - 1) / 2;
		for (int i = 0; i < char_size; i++)
			if (here->next[i] != nullptr && here->cnt[i] > 1)
				res += count_internal(here->next[i]);
	}
	int char_set(char x) {
		return std::lower_bound(char_set_press.begin(), char_set_press.end(), x) -
					 char_set_press.begin();
	}
	int char_size;
	std::vector<char> char_set_press;

public:
	trie(char first_char, int size) {
		char_size = size;
		tree = new node(size);
		for (int i = 0; i < size; i++)
			char_set_press.push_back((char)(first_char + i));
	}
	trie(std::vector<char> &char_list) {
		char_size = char_list.size();
		tree = new node(char_size);
		char_set_press = char_list;
		sort(char_set_press.begin(), char_set_press.end());
	}
	void insert(std::string &s) {
		node *here = tree;
		for (auto x : s) {
			int c = char_set(x);
			here->cnt[c]++;
			if (here->next[c] == nullptr)
				here->next[c] = new node(char_size);
			here = here->next[c];
		}
	} 
	// ABC 353 E https://atcoder.jp/contests/abc353/tasks/abc353_e
	int count_prefix_sum() { return count_prefix_internal(tree); }
	// ABC 287 E https://atcoder.jp/contests/abc287/tasks/abc287_e
	int count_longest_prefix(std::string&s){
		int l=0;
		node*here=tree;
		for(auto x:s){
			int c=char_set(x);
			if(here->next[c]==nullptr||here->cnt[c]<2)
				break;
			here=here->next[c];
			l++;
		}
		return l;
	}
};