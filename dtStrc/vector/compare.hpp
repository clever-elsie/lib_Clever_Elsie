#ifndef ELSIE_VECTOR_COMPARE
#define ELSIE_VECTOR_COMPARE

namespace elsie{

template<class U,class V>
requires requires(U x,V y){ x==y; }
bool operator==(const vector<U>&lhs,const vector<V>&rhs){
  if(lhs.size()!=rhs.size())return false;
  using size_type=typename vector<U>::size_type;
  for(size_type i=0;i<lhs.size();++i)
    if(lhs.data_[i]!=rhs.data_[i])
      return false;
  return true;
}

template<class U,class V>
requires requires(U x,V y){ x<y; x==y; }
bool operator<(const vector<U>&lhs,const vector<V>&rhs){
  using size_type=typename vector<U>::size_type;
  size_type n=std::min(lhs.size(),rhs.size());
  for(size_type i=0;i<n;++i){
    if(lhs.data_[i]==rhs.data_[i])continue;
    return(lhs.data_[i]<rhs.data_[i]);
  }
  return lhs.size()<rhs.size();
}

template<class U,class V>
requires requires(U x,V y){ x>y; x==y; }
bool operator>(const vector<U>&lhs,const vector<V>&rhs){
  using size_type=typename vector<U>::size_type;
  size_type n=std::min(lhs.size(),rhs.size());
  for(size_type i=0;i<n;++i){
    if(lhs.data_[i]==rhs.data_[i])continue;
    return(lhs.data_[i]>rhs.data_[i]);
  }
  return lhs.size()>rhs.size();
}

template<class U,class V>
requires requires(U x,V y){ x<=y; x==y; }
bool operator<=(const vector<U>&lhs,const vector<V>&rhs){
  using size_type=typename vector<U>::size_type;
  size_type n=std::min(lhs.size(),rhs.size());
  for(size_type i=0;i<n;++i){
    if(lhs.data_[i]==rhs.data_[i])continue;
    return(lhs.data_[i]<rhs.data_[i]);
  }
  return lhs.size()<=rhs.size();
}

template<class U,class V>
requires requires(U x,V y){ x>=y; x==y; }
bool operator>=(const vector<U>&lhs,const vector<V>&rhs){
  using size_type=typename vector<U>::size_type;
  size_type n=std::min(lhs.size(),rhs.size());
  for(size_type i=0;i<n;++i){
    if(lhs.data_[i]==rhs.data_[i])continue;
    return(lhs.data_[i]>rhs.data_[i]);
  }
  return lhs.size()>=rhs.size();
}

} // namespace elsie

#endif