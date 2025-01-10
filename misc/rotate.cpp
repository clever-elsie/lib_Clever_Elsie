#include <vector>
#include <cstddef>
using namespace std;

// in local.h
template<class T>concept Itrabl=requires(const T&x){x.begin();x.end();};
template<class T>concept IItrabl=Itrabl<T>&&Itrabl<typename T::value_type>;

// vector, dequeの組合せを想定
// IItrablに加えて.resize(), operator[]が必要。
// array, list, set, map, unordered_(set|map), native pointerなどは使用不可

// 長方形(正方形含む)限定
template<size_t rot=0,IItrabl T>
void vvRotate(T&v){
	size_t h=v.size(),w=v.at(0).size();
	T t=move(v);
	v.resize(w);
	for(int i=0;i<w;i++){
		v[i].resize(h,typename T::value_type::value_type());
		for(int j=0;j<h;j++)
			if constexpr (rot) v[i][j]=t[h-1-j][i]; // R rotate (clock)
			else v[i][j]=t[j][w-1-i]; // L rotate (inv clock)
	}
}

// 一般のvvに使える
template<size_t rot=0,IItrabl T>
void vvRotate(T&v){
	size_t h=v.size(),w=0;
	for(const auto&x:v)w=max(w,x.size());
	T t=move(v);
	v.resize(w);
	for(int i=0;i<w;i++){
		v[i].resize(h,typename T::value_type::value_type());
		for(int j=0;j<h;j++)
			if constexpr (rot) v[i][j]=t[h-1-j][i]; // R rotate (clock)
			else v[i][j]=t[j][w-1-i]; // L rotate (inv clock)
	}
}