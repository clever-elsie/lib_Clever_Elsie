#ifndef ELSIE_FLAT_VECTOR
#define ELSIE_FLAT_VECTOR
#include <array>
#include <vector>
#include <ranges>
#include <concepts>
#include <type_traits>
#include <exception>
#include <stdexcept>
namespace elsie{

template<class T,size_t dimension>
class fvec{

static_assert(dimension,"dimension must be 1 or greater than it.\n");

protected:
using asd=std::array<size_t,dimension>;
using asd1=std::array<size_t,dimension+1>;

std::vector<T>data;
asd dims;
asd1 units;
inline static T out_of_range=T();
inline const static T out_of_range_const=T();

inline void calc_unit(const asd&d){
  units.back()=1;
  for(ssize_t i=dimension-1;i>=0;--i)
    units[i]=units[i+1]*d[i];
}

inline void rec_copy(size_t&idx,size_t di,const auto&v){
  if(di==dimension-1)
    for(size_t i=0;i<dims[di];++i)
      data[idx++]=v[i];
  else for(size_t i=0;i<dims[di];++i)
    rec_copy(idx,di+1,v[i]);
}

public:
using value_type=T;
using reference=T&;
using const_reference=const T&;
using iterator=std::vector<T>::iterator;
using const_iterator=std::vector<T>::const_iterator;
using reverse_iterator=std::vector<T>::reverse_iterator;
using const_reverse_iterator=std::vector<T>::const_reverse_iterator;
fvec(const fvec&)=default;
fvec(fvec&&)=default;
~fvec()=default;
fvec(const asd&dim_):dims(dim_),units(),data(0){
    calc_unit(dim_);
    data.resize(units[0]);
    fill(T());
  }
fvec(const asd&dim_,vector<T>&&aligned_vec):dims(dim_),units(),data(aligned_vec){ calc_unit(dim_); }
fvec(const asd&dim_,const vector<T>&aligned_vec):dims(dim_),units(),data(aligned_vec){ calc_unit(dim_); }
fvec(const asd&dim_,const auto&v):dims(dim_),units(),data(units[0]){
  calc_unit(dim_);
  size_t i=0;
  rec_copy(i,0,v);
}

size_t size()const{return units[0];}
size_t size(size_t d)const{return dims[d];}

iterator begin(){return data.begin();}
iterator end(){return data.end();}
const_iterator begin()const{return data.cbegin();}
const_iterator end()const{return data.cend();}
const_iterator cbegin()const{return data.cbegin();}
const_iterator cend()const{return data.cend();}
reverse_iterator rbegin(){return data.rbegin();}
reverse_iterator rend(){return data.rend();}
const_reverse_iterator rbegin()const{return data.crbegin();}
const_reverse_iterator rend()const{return data.crend();}
const_reverse_iterator crbegin()const{return data.crbegin();}
const_reverse_iterator crend()const{return data.crend();}

template<class... Args>
  requires (sizeof...(Args)==dimension)
  && (std::conjunction_v<std::is_convertible<Args,size_t>...>)
reference operator[](Args... args){
  asd indices{static_cast<size_t>(args)...};
  size_t idx=0;
  for(size_t i=0;i<dimension;++i)
    if(indices[i]>=dims[i])
      return out_of_range=T();
  if constexpr(dimension==2) return data[units[1]*indices[0]+indices[1]];
  if constexpr(dimension==3) return data[units[2]*indices[0]+units[1]*indices[1]+indices[2]];
  else{
    for(size_t i=0;i<dimension-1;++i)
      idx+=units[i+1]*indices[i];
    // last element of unit is allways 1, so that no need multiply
    return data[idx+indices.back()];
  }
}

template<class... Args>
  requires (sizeof...(Args)==dimension)
  && (std::conjunction_v<std::is_convertible<Args,size_t>...>)
const_reference operator[](Args... args)const{
  asd indices{static_cast<size_t>(args)...};
  size_t idx=0;
  for(size_t i=0;i<dimension;++i)
    if(indices[i]>=dims[i])
      return out_of_range_const;
  if constexpr(dimension==2) return data[units[1]*indices[0]+indices[1]];
  if constexpr(dimension==3) return data[units[2]*indices[0]+units[1]*indices[1]+indices[2]];
  else{
    for(size_t i=0;i<dimension-1;++i)
      idx+=units[i+1]*indices[i];
    // last element of unit is allways 1, so that no need multiply
    return data[idx+indices.back()];
  }
}

template<class... Args>
  requires (sizeof...(Args)==dimension)
  && (std::conjunction_v<std::is_convertible<Args,size_t>...>)
reference at(Args... args){
  asd indices{static_cast<size_t>(args)...};
  for(size_t i=0;i<dimension;++i)
    if(indices[i]>=dims[i]) throw std::out_of_range();
  size_t idx=0;
  for(size_t i=0;i<dimension-1;++i)
    idx+=units[i+1]*indices[i];
  idx+=indices.back(); // last element of unit is allways 1, so that no need multiply
  return data[idx];
}
template<class... Args>
  requires (sizeof...(Args)==dimension)
  && (std::conjunction_v<std::is_convertible<Args,size_t>...>)
const_reference at(Args... args)const{
  asd indices{static_cast<size_t>(args)...};
  for(size_t i=0;i<dimension;++i)
    if(indices[i]>=dims[i])throw std::out_of_range();
  size_t idx=0;
  for(size_t i=0;i<dimension-1;++i)
    idx+=units[i+1]*indices[i];
  idx+=indices.back(); // last element of unit is allways 1, so that no need multiply
  return data[idx];
}

void fill(const T&val){
  for(size_t i=0;i+16<data.size();i+=16){
    data[i+ 0]=T(), data[i+ 1]=T();
    data[i+ 2]=T(), data[i+ 3]=T();
    data[i+ 4]=T(), data[i+ 5]=T();
    data[i+ 6]=T(), data[i+ 7]=T();
    data[i+ 8]=T(), data[i+ 9]=T();
    data[i+10]=T(), data[i+11]=T();
    data[i+12]=T(), data[i+13]=T();
    data[i+14]=T(), data[i+15]=T();
  }
  constexpr static size_t bitmask=~0xFULL;
  for(auto&x:data|std::views::drop(data.size()&bitmask)) x=T();
}

template<class RHS>
fvec&operator=(RHS&&rhs){
  static_assert(std::is_same_v<std::decay_t<RHS>,fvec>);
  if(dims!=rhs.dims)throw std::invalid_argument("fvec dimensions missmatch");
  data=forward<RHS>(rhs.data);
  return*this;
}
}; // class fvec

}//namespace elsie
#endif