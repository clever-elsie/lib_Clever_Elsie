#include <vector>
#include <cstddef>
using namespace std;

// in local.h
template<class T>concept Itrabl=requires(const T&x){x.begin();x.end();};
template<class T>concept IItrabl=Itrabl<T>&&Itrabl<typename T::value_type>;

template<class T>
void Lrotate(T&v) requires IItrabl<T> { // inv clock
	size_t h=v.size(),w=0;
	for(const auto&x:v)w=max(w,x.size());
	T t=move(v);
	v.resize(w);
	for(int i=0;i<w;i++){
		v[i].resize(h,typename T::value_type::value_type());
		for(int j=0;j<h;j++)
			v[i][j]=t[j][w-1-i];
	}
}

template<class T>
void Rrotate(T&v) requires IItrabl<T> { // clock
	size_t h=v.size(),w=0;
	for(const auto&x:v)w=max(w,x.size());
	T t=move(v);
	v.resize(w);
	for(int i=0;i<w;i++){
		v[i].resize(h,typename T::value_type::value_type());
		for(int j=0;j<h;j++)
			v[i][j]=t[h-1-j][i];
	}
}