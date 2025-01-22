#include <utility> // forward(), move()
#include <any> // swap()
#include <functional> // less<>
#include <initializer_list>
#include <cstdint>
#include <cstddef> // size_t

template<class key_t,class val_t,class cmp=std::less<key_t>>
class tree{
	pubilc:
	using std::size_t;
	using value_type=std::pair<const key_t,val_t>;
	protected:
	struct node{
		node *l,*r,*par,*nxt,*prv;
		value_type*v;
		size_t h,sz;
		node(value_type*val,node*p=nullptr,node*nx=nullptr,node*pr=nullptr):v(val),par(p),nxt(nx),prv(pr){}
		~node(){ delete v; }
	};
	size_t cur_size;
	node*root;
	iterator insert(value_type*p){
		if(root==nullptr){
			root=new node(p);
			cur_size=1;
			return iterator(node);
		}
	}
	public:
	class iterator{
		private:node*rf;
		public:iterator(node*x=nullptr):rf(x){}
		bool operator==(iterator p){}
		bool operator!=(iterator p){}
		iterator&operator++(){}
		iterator operator++(int32_t){}
		iterator&operator--(){}
		iterator operator--(int32_t){}
	};
	// constructor
	tree():root(nullptr),cur_size(0){}
	tree(std::initializer_list<value_type>init){}
	// access
	iterator find(const key_t&key){}
	bool contains(const key_t&key){return find(key)!=end();}
	size_t count(const key_t&key){return contains(key);}
	val_t&operator[](const key_t&key){}
	iterator lower_bound(const key_t&key){}
	iterator upper_bound(const key_t&key){}
	size_t order_of_key(const key_t&key){}
	iterator find_by_order(size_t idx){}
	// update
	iterator insert(value_type&&x){
		value_type*p=new value_type(std::forward(x));
		return insert(p);
	}
	iterator emplace(key_t&&key,val_t&&val){
		value_type*p=new value_type(std::forward(key),std::forward(val));
		return insert(p);
	}
	iterator erase(const key_t&key){
		iterator p=find(key),r=p;
		if(p==end())return end();
		++r;erase(p);
		return r;
	}
	iterator erase(iterator&itr){

	}
	void clear(){
		node*p=root;
		while(p!=nullptr){
			node*nx=nullptr;
			if(p->l==nullptr&&p->r==nullptr){
				nx=p->par;
				delete p;
			}else if(p->l!=nullptr) nx=p->l;
			else if(p->r!=nullptr) nx=p->r;
			p=nx;
		}
		root=nullptr;
		cur_size=0;
	}
	friend void swap(tree&x){
		swap(cur_size,x.cur_size);
		swpa(root,x.root);
	}
	void merge(tree&x){}
	//iterator
	iterator begin(){}
	constexpr iterator end(){return iterator();}
	iterator rbegin(){}
	iterator rend(){}
	//memory
	bool empty(){}
	size_t size(){}

};

template<class key_t,class val_t,class hash_t=std::hash<key_t>>
class hashmap{
};