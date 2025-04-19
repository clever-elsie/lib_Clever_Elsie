#ifndef ELSIE_STRASSEN
#define ELSIE_STRASSEN
#include <new>
#include <array>
#include <vector>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <concepts>
#include <type_traits>
#include <utility>
namespace elsie{
using namespace std;

template<class T> class matrix{
  using ps=pair<size_t,size_t>;
  template<class S>using vc=vector<S>;
  template<class S>using vv=vc<vc<S>>;
  size_t row,col;
  vc<T> data;
  public:
  matrix()=default;
	~matrix()=default;
  matrix(matrix&&)=default;
  matrix(const matrix&)=default;
	matrix&operator=(matrix&&)=default;
	matrix&operator=(const matrix&)=default;

  matrix(size_t row_,size_t col_);
  matrix(size_t row_,size_t col_,const T&init);
  template<class S> matrix(size_t row_,size_t col_,S&&data_) requires std::same_as<std::decay_t<S>,vc<T>>;
  template<size_t row_,size_t col_> matrix(const array<array<T,col_>,row_>&data_);
  template<class T> matrix(const vv<T>&data_);

  inline pair<size_t,size_t>size()const{return {row,col};}
  inline T& operator[](size_t i,size_t j){ return data[i*col+j]; }
  inline const T& operator[](size_t i,size_t j)const{ return data[i*col+j]; }

  void transpose();
  matrix<T> pow(uint64_t b)const;
  friend std::ostream& operator<<(std::ostream&os,const matrix&rhs);

  using iterator=vc<T>::iterator;
  using const_iterator=vc<T>::const_iterator;
  using reverse_iterator=vc<T>::reverse_iterator;
  using const_reverse_iterator=vc<T>::const_reverse_iterator;
  inline iterator begin(){return data.begin();}  
  inline iterator end(){return data.end();}
  inline const_iterator begin()const{return data.cbegin();}
  inline const_iterator end()const{return data.cend();}
  inline const_iterator cbegin()const{return data.cbegin();}
  inline const_iterator cend()const{return data.cend();}
  inline reverse_iterator rbegin(){return data.rbegin();}
  inline reverse_iterator rend(){return data.rend();}
  inline const_reverse_iterator rbegin()const{return data.crbegin();}
  inline const_reverse_iterator rend()const{return data.crend();}
  inline const_reverse_iterator crbegin()const{return data.crbegin();}
  inline const_reverse_iterator crend()const{return data.crend();}


  // スカラ倍
  template<class U> matrix& operator*=(const U&);
  template<class U> matrix& operator/=(const U&);
	template<class U,class V,class W> friend matrix<W>& operator*(const U&,const matrix<V>&);
	template<class U,class V,class W> friend matrix<W>& operator*(const matrix<U>&,const V&);
	template<class U,class V,class W> friend matrix<W>& operator/(const matrix<U>&,const V&);

  // 行列演算
  template<class U> matrix& operator+=(const matrix<U>&);
  template<class U> matrix& operator-=(const matrix<U>&);
  template<class U> matrix& operator*=(const matrix<U>&);
  template<class U,class V,class W> friend matrix<W> operator+(const matrix<U>&,const matrix<V>&);
  template<class U,class V,class W> friend matrix<W> operator-(const matrix<U>&,const matrix<V>&);
  template<class U,class V,class W> friend matrix<W> operator*(const matrix<U>&,const matrix<V>&);

	protected:
  /**
   * @brief a:NxN matrix. N is 2^i and N>1.
   * @return {A_00, A_01, A_10, A_11}
   */
  vc<matrix>split(size_t n)const;
  static matrix merge(const matrix&,const matrix&,const matrix&,const matrix&);

  template<size_t naive,size_t base,bool q2,class U,class V,class W>
  friend matrix<W> matrix_mul(const matrix<U>&,const matrix<V>&);

}; // class matrix
 
///////////////////
///////////////////
///////////////////
/// matrix impl ///
///////////////////
///////////////////
///////////////////

template<class T>
matrix<T>::matrix(size_t row_,size_t col_)
:row(row_),col(col_),data(row*col){}

template<class T>
matrix<T>::matrix(size_t row_,size_t col_,const T&init)
:row(row_),col(col_),data(row_*col_,init){}

template<class T>
template<class S>
requires std::same_as<std::decay_t<S>,vc<T>>
matrix<T>::matrix<T>(size_t row_,size_t col_,S&&data_)
:row(row_),col(col_),data(std::forward<S>(data_)){ assert(row&&col); }

template<class T>
template<size_t row_,size_t col_>
matrix<T>::matrix(const std::array<std::array<T,col_>,row_>&data_)
:row(row_),col(col_),data(row*col){
  static_assert(row&&col);
  auto dtr=data.begin();
  for(const auto&y:data_)
  for(const auto&x:y) *(dtr++)=x;
}


template<class T>
matrix<T>::matrix(const std::vector<std::vector<<T>>&data_)
:row(data_.size()),col(data_[0].size()),data(row*col){
  assert(row&&col);
  auto dtr=data.begin();
  for(const auto&y:data_){
    assert(y.size()==col);
    for(const auto&x:y) *(dtr++)=x;
  }
}

template<class T>
template<class U>
matrix<T>& matrix<T>::operator+=(const matrix<U>&rhs){
  assert(row==rhs.row&&col==rhs.col);
  constexpr size_t unrool=16-1;
  for(size_t i=0;i+unrool<data.size();i+=(unrool+1)){
    data[i   ]+=rhs.data[i   ];
    data[i+ 1]+=rhs.data[i+ 1];
    data[i+ 2]+=rhs.data[i+ 2];
    data[i+ 3]+=rhs.data[i+ 3];
    data[i+ 4]+=rhs.data[i+ 4];
    data[i+ 5]+=rhs.data[i+ 5];
    data[i+ 6]+=rhs.data[i+ 6];
    data[i+ 7]+=rhs.data[i+ 7];
    data[i+ 8]+=rhs.data[i+ 8];
    data[i+ 9]+=rhs.data[i+ 9];
    data[i+10]+=rhs.data[i+10];
    data[i+11]+=rhs.data[i+11];
    data[i+12]+=rhs.data[i+12];
    data[i+13]+=rhs.data[i+13];
    data[i+14]+=rhs.data[i+14];
    data[i+15]+=rhs.data[i+15];
  }
  for(size_t i=data.size()&(~unrool);i<data.size();++i)
    data[i]+=rhs.data[i];
  return*this;
}

template<class T>
template<class U>
matrix<T>& matrix<T>::operator-=(const matrix<U>&rhs){
  assert(row==rhs.row&&col==rhs.col);
  constexpr size_t unrool=16-1;
  for(size_t i=0;i+unrool<data.size();i+=(unrool+1)){
    data[i   ]-=rhs.data[i   ];
    data[i+ 1]-=rhs.data[i+ 1];
    data[i+ 2]-=rhs.data[i+ 2];
    data[i+ 3]-=rhs.data[i+ 3];
    data[i+ 4]-=rhs.data[i+ 4];
    data[i+ 5]-=rhs.data[i+ 5];
    data[i+ 6]-=rhs.data[i+ 6];
    data[i+ 7]-=rhs.data[i+ 7];
    data[i+ 8]-=rhs.data[i+ 8];
    data[i+ 9]-=rhs.data[i+ 9];
    data[i+10]-=rhs.data[i+10];
    data[i+11]-=rhs.data[i+11];
    data[i+12]-=rhs.data[i+12];
    data[i+13]-=rhs.data[i+13];
    data[i+14]-=rhs.data[i+14];
    data[i+15]-=rhs.data[i+15];
  }
  for(size_t i=data.size()&(~unrool);i<data.size();++i)
    data[i]-=rhs.data[i];
  return*this;
}

template<class T>
template<class U>
matrix& matrix<T>::operator*=(const U&rhs){
  for(auto&x:data)x*=rhs;
  return*this;
}

template<class U,class V,class W=common_type_t<U,V>>
friend matrix<W>& operator*(const U&lhs,const matrix<V>&rhs){
  auto [row,col]=rhs.size();
  size_t sz=rhs.data.size();
  std::vector<W> data;
  data.reserve(sz);
  for(const auto&x:rhs)
    data.emplace_back(lhs*x);
  return matrix<W>(row,col,std::move(data));
}

template<class U,class V,class W=common_type_t<U,V>>
friend matrix<W>& operator*(const matrix<U>&lhs,const V&rhs){
  auto [row,col]=lhs.size();
  size_t sz=lhs.data.size();
  std::vector<W> data;
  data.reserve(sz);
  for(const auto&x:lhs)
    data.emplace_back(x*rhs);
  return matrix<W>(row,col,std::move(data));
}

template<class T>
template<class U>
matrix<T>& matrix<T>::operator/=(const U&rhs){
  for(auto&x:data)x/=rhs;
  return*this;
}

template<class T>
template<class U>
matrix<T>& matrix<T>::operator*=(const matrix<U>&rhs){
  assert(col==rhs.col&&col==rhs.row);
  *this=matrix_mul(*this,rhs);
  return*this;
}

template<class U,class V,class W=common_type_t<U,V>>
friend matrix<W>& operator/(const matrix<U>&lhs,const V&rhs){
  auto [row,col]=lhs.size();
  size_t sz=lhs.data.size();
  std::vector<W> data;
  data.reserve(sz);
  for(const auto&x:lhs)
    data.emplace_back(x/rhs);
  return matrix<W>(row,col,std::move(data));
}

template<class U,class V,class W=std::common_type_t<U,V>>
friend matrix<W> operator+(const matrix<U>&lhs,const matrix<V>&rhs){
  assert(lhs.row==rhs.row&&lhs.col==rhs.col);
  matrix<W> res(lhs.row,lhs.col);
  constexpr size_t unrool=16-1;
  for(size_t i=0;i+unrool<res.data.size();i+=(unrool+1)){
    res.data[i   ]=lhs.data[i   ]+rhs.data[i   ];
    res.data[i+ 1]=lhs.data[i+ 1]+rhs.data[i+ 1];
    res.data[i+ 2]=lhs.data[i+ 2]+rhs.data[i+ 2];
    res.data[i+ 3]=lhs.data[i+ 3]+rhs.data[i+ 3];
    res.data[i+ 4]=lhs.data[i+ 4]+rhs.data[i+ 4];
    res.data[i+ 5]=lhs.data[i+ 5]+rhs.data[i+ 5];
    res.data[i+ 6]=lhs.data[i+ 6]+rhs.data[i+ 6];
    res.data[i+ 7]=lhs.data[i+ 7]+rhs.data[i+ 7];
    res.data[i+ 8]=lhs.data[i+ 8]+rhs.data[i+ 8];
    res.data[i+ 9]=lhs.data[i+ 9]+rhs.data[i+ 9];
    res.data[i+10]=lhs.data[i+10]+rhs.data[i+10];
    res.data[i+11]=lhs.data[i+11]+rhs.data[i+11];
    res.data[i+12]=lhs.data[i+12]+rhs.data[i+12];
    res.data[i+13]=lhs.data[i+13]+rhs.data[i+13];
    res.data[i+14]=lhs.data[i+14]+rhs.data[i+14];
    res.data[i+15]=lhs.data[i+15]+rhs.data[i+15];
  }
  for(size_t i=res.data.size()&(~unrool);i<res.data.size();++i)
    res.data[i]=lhs.data[i]+rhs.data[i];
  return res;
}

template<class U,class V,class W=std::common_type_t<U,V>>
friend matrix<W> operator-(const matrix<U>&lhs,const matrix<V>&rhs){
  assert(lhs.row==rhs.row&&lhs.col==rhs.col);
  matrix<W> res(lhs.row,lhs.col);
  constexpr size_t unrool=16-1;
  for(size_t i=0;i+unrool<res.data.size();i+=(unrool+1)){
    res.data[i   ]=lhs.data[i   ]-rhs.data[i   ];
    res.data[i+ 1]=lhs.data[i+ 1]-rhs.data[i+ 1];
    res.data[i+ 2]=lhs.data[i+ 2]-rhs.data[i+ 2];
    res.data[i+ 3]=lhs.data[i+ 3]-rhs.data[i+ 3];
    res.data[i+ 4]=lhs.data[i+ 4]-rhs.data[i+ 4];
    res.data[i+ 5]=lhs.data[i+ 5]-rhs.data[i+ 5];
    res.data[i+ 6]=lhs.data[i+ 6]-rhs.data[i+ 6];
    res.data[i+ 7]=lhs.data[i+ 7]-rhs.data[i+ 7];
    res.data[i+ 8]=lhs.data[i+ 8]-rhs.data[i+ 8];
    res.data[i+ 9]=lhs.data[i+ 9]-rhs.data[i+ 9];
    res.data[i+10]=lhs.data[i+10]-rhs.data[i+10];
    res.data[i+11]=lhs.data[i+11]-rhs.data[i+11];
    res.data[i+12]=lhs.data[i+12]-rhs.data[i+12];
    res.data[i+13]=lhs.data[i+13]-rhs.data[i+13];
    res.data[i+14]=lhs.data[i+14]-rhs.data[i+14];
    res.data[i+15]=lhs.data[i+15]-rhs.data[i+15];
  }
  for(size_t i=res.data.size()&(~unrool);i<res.data.size();++i)
    res.data[i]=lhs.data[i]-rhs.data[i];
  return res;
}

template<class U,class V,class W=std::common_type_t<U,V>>
friend matrix<W> operator*(const matrix<U>&lhs,const matrix<V>&rhs){
  return matrix_mul(lhs,rhs);
}

template<class T>
void matrix<T>::transpose(){
  vc<T> res(col*row);
  constexpr const size_t block_size=64;
  size_t i,j,I,J,Erow,Ecol;
  for(i=0,Erow=min(block_size,row);i<row;Erow=min(i+=block_size,row))
  for(j=0,Ecol=min(block_size,col);j<col;Ecol=min(j+=block_size,col))
    for(I=i;I<Erow;++I) for(J=j;J<Ecol;++J)
      res[J*col+I]=data[I*row+J];
  std::swap(row,col);
  data=std::move(res);
}

template<class T>
std::vector<matrix<T>> matrix<T>::split(size_t n)const{
  assert(std::popcount(n)==1);
  size_t n2=n>>1,filter=n2-1;
  uint32_t shift=popcount(filter);
  std::vector<matrix<T>>r(4,matrix(n2,n2,T()));
  for(size_t i=0;i<row;++i)
  for(size_t j=0;j<col;++j){
      size_t idx=((i>=n2)<<1)+(j>=n2);
      size_t li=i&filter, lj=j&filter;
      r[idx].data[(li<<shift)+lj]=data[i*col+j];
  }
  return r;
}

template<class T> static
matrix<T> matrix<T>::merge(const matrix<T>&c11,const matrix<T>&c12,const matrix<T>&c21,const matrix<T>&c22){
  // n satisfy n=2^c, n>=256
  const size_t n=c11.row;
  matrix<T> c(n*2,n*2);
  for(size_t i=0,j;i<n;++i){
    size_t I=i*n*2,ii=i*n;
    std::copy(c11.data.begin()+ii,c11.data.begin()+ii+n,c.data.begin()+I);
    std::copy(c12.data.begin()+ii,c12.data.begin()+ii+n,c.data.begin()+I+n);
  }
  for(size_t i=0,j;i<n;++i){
    size_t I=(i+n)*n*2,ii=i*n;
    std::copy(c21.data.begin()+ii,c21.data.begin()+ii+n,c.data.begin()+I);
    std::copy(c22.data.begin()+ii,c22.data.begin()+ii+n,c.data.begin()+I+n);
  }
  return c;
}

namespace{
template<size_t naive=64,size_t base=256,bool q2=true,class U,class V,class W=common_type_t<U,V>>
friend matrix<W> matrix_mul(const matrix<U>&a,const matrix<V>&b){
  assert(a.col==b.row);
  size_t N=a.row,M=b.row,L=b.col;
  auto[Q,P]=minmax({N,M,L});
  if(P<base){
    matrix<W>c(N,L,W());
    if(Q<naive){
      uint32_t i,j,k;
      uint32_t x,y,z;
      constexpr const uint16_t unrool=8-1;
      for(i=0,x=0,y=0;i<N;++i,x+=c.col,y+=a.col)
      for(k=0,z=0;k<M;++k,z+=b.col){
        for(j=0;j+unrool<L;j+=(unrool+1)){
          c.data[x+j  ]+=a.data[y+k]*b.data[z+j  ];
          c.data[x+j+1]+=a.data[y+k]*b.data[z+j+1];
          c.data[x+j+2]+=a.data[y+k]*b.data[z+j+2];
          c.data[x+j+3]+=a.data[y+k]*b.data[z+j+3];
          c.data[x+j+4]+=a.data[y+k]*b.data[z+j+4];
          c.data[x+j+5]+=a.data[y+k]*b.data[z+j+5];
          c.data[x+j+6]+=a.data[y+k]*b.data[z+j+6];
          c.data[x+j+7]+=a.data[y+k]*b.data[z+j+7];
        }
        for(j=L&(~unrool);j<L;++j)
          c.data[x+j]+=a.data[y+k]*b.data[z+j];
      }
    }else{
      constexpr static uint16_t bs=naive;
      constexpr static uint16_t unrool=16-1;
      uint16_t I,J,K,i,j,k,it,jt,kt;
      for(I=0,it=bs;I<N;I+=bs,it=min<uint16_t>(it+bs,N))
      for(J=0,jt=bs;J<L;J+=bs,jt=min<uint16_t>(jt+bs,L))
      for(K=0,kt=bs;K<M;K+=bs,kt=min<uint16_t>(kt+bs,M))
        for(i=I;i<it;++i){
          uint16_t ci=i*c.col,ai=i*a.col+K;
          for(k=K;k<kt;++k,++ai){
            uint16_t bi=k*b.col;
            for(j=J;j+unrool<jt;j+=(unrool+1)){
              c.data[ci+j   ]+=a.data[ai]*b.data[bi+j   ];
              c.data[ci+j+ 1]+=a.data[ai]*b.data[bi+j+ 1];
              c.data[ci+j+ 2]+=a.data[ai]*b.data[bi+j+ 2];
              c.data[ci+j+ 3]+=a.data[ai]*b.data[bi+j+ 3];
              c.data[ci+j+ 4]+=a.data[ai]*b.data[bi+j+ 4];
              c.data[ci+j+ 5]+=a.data[ai]*b.data[bi+j+ 5];
              c.data[ci+j+ 6]+=a.data[ai]*b.data[bi+j+ 6];
              c.data[ci+j+ 7]+=a.data[ai]*b.data[bi+j+ 7];
              c.data[ci+j+ 8]+=a.data[ai]*b.data[bi+j+ 8];
              c.data[ci+j+ 9]+=a.data[ai]*b.data[bi+j+ 9];
              c.data[ci+j+10]+=a.data[ai]*b.data[bi+j+10];
              c.data[ci+j+11]+=a.data[ai]*b.data[bi+j+11];
              c.data[ci+j+12]+=a.data[ai]*b.data[bi+j+12];
              c.data[ci+j+13]+=a.data[ai]*b.data[bi+j+13];
              c.data[ci+j+14]+=a.data[ai]*b.data[bi+j+14];
              c.data[ci+j+15]+=a.data[ai]*b.data[bi+j+15];
            }
            for(j=jt&(~unrool);j<jt;++j)
              c.data[ci+j]+=a.data[ai]*b.data[bi+j];
          }
        }
    }
    return c;
  }else{
    size_t n;
    if constexpr(q2) n=size_t(1)<<(63-countl_zero(P)+(popcount(P)!=1));
    else n=L;
    std::vector<matrix<U>> as=a.split(n);
    std::vector<matrix<V>> bs=b.split(n);
    matrix<U> s2=as[0]+as[1], s9=as[0]-as[2], s5=as[0]+as[3], s3=as[2]+as[3], s7=as[1]-as[3];
    matrix<V> s1=bs[1]-bs[3], s8=bs[2]+bs[3], s6=bs[0]+bs[3], s4=bs[2]-bs[0], sA=bs[0]+bs[1];

    matrix<W> p1=matrix_mul<naive,base,false>(as[0],s1);
    matrix<W> p2=matrix_mul<naive,base,false>(s2,bs[3]);
    matrix<W> p3=matrix_mul<naive,base,false>(s3,bs[0]);
    matrix<W> p4=matrix_mul<naive,base,false>(as[3],s4);
    matrix<W> p5=matrix_mul<naive,base,false>(s5,s6);
    matrix<W> p6=matrix_mul<naive,base,false>(s7,s8);
    matrix<W> p7=matrix_mul<naive,base,false>(s9,sA);

    auto r=matrix<W>::merge(
      ((p5+p4)-=p2)+=p6,p1+p2,
      p3+p4, ((p5+p1)-=p3)-=p7
    );
    if constexpr(q2)
      if(N!=r.row&&L!=r.col){
        matrix ret(N,L);
        for(size_t i=0;i<N;++i)
        for(size_t j=0;j<L;++j)
          ret[i,j]=r[i,j];
        r=std::move(ret);
      }
    return r;
  }
}
} // namespace

template<class T>
matrix<T> pow(uint64_t b)const{
  assert(b<=1||row==col);
  matrix<T> r(row,row,T());
  for(size_t i=0;i<row;++i) r[i,i]=1;
  matrix<T> k(*this);
  for(;b;b>>=1){
    if(b&1) r=r*k;
    k=k*k;
  }
  return r;
}

friend std::ostream& operator<<(std::ostream&os,const matrix&rhs){
  size_t cnt=0;
  for(const auto&x:rhs){
    bool nl=++cnt==rhs.col;
    os<<x<<" \n"[nl];
    if(nl)cnt=0;
  }
  return os;
}

}// namespace elsie
#endif