#include <vector>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <utility>

namespace elsie{

template<class T>
struct GaussianReduction{
  private:
  using vc = std::vector<T>;
  using vv = std::vector<vc>;
  public:
  GaussianReduction()=default;
  // Ax=b
  vc operator()(const vv&a, const vc&b)noexcept(false){
    if(a.size()!=b.size()) throw std::runtime_error("GaussianReduction: different row size.");
    if(a.size()==0) return {};
    ac.clear();
    ac.push_back(a[0]);
    ac[0].push_back(b[0]);
    std::size_t col=a[0].size();
    for(size_t i=1;i<a.size();++i){
      ac.push_back(a[i]);
      ac[i].push_back(b[i]);
      if(ac[i].size()!=col)
        throw std::runtime_error("GaussianReduction: different col size.");
    }
    try{ return impl(); }catch(const std::runtime_error&e){ throw e; }
  }
  vc operator()(const vv&ab)noexcept(false){
    ac=ab;
    try{ return impl(); }catch(const std::runtime_error&e){ throw e; }
  }
  vc operator()(vv&&ab)noexcept(false){
    ac=std::move(ab);
    try{ return impl(); }catch(const std::runtime_error&e){ throw e; }
  }
  vv basis(){
    std::size_t row=ac.size();
    if(row==0) throw std::runtime_error("GaussianReduction: no basis");
    std::size_t col=ac[0].size()-1;
    std::size_t dim=col-row;
    vv b(dim,vc(col,0));
		std::size_t start=0;
		std::vector<std::int32_t> p(row);
		std::vector<bool> is_main(col,0);
		for(std::size_t i=0;i<row;++i){
			while(ac[i][start]==0)++start;
			is_main[start]=true;
			p[i]=start;
		}
		std::vector<std::int32_t> q(col,-1);
		for(std::size_t j=0,cur=0;j<col;++j)
			if(!is_main[j])
				q[j]=cur++;
		//out(is_main,q,ac);
		for(std::size_t j=0;j<col;++j)if(q[j]>=0)
			b[q[j]][j]=1;
		for(std::size_t i=0;i<row;++i){
			for(std::size_t j=0;j<col;++j)if(q[j]>=0){
				b[q[j]][p[i]]=-ac[i][j];
			}
		}
    return b;
  }
  private:
  vv ac;
  vc impl() noexcept(false) {
    const std::size_t orow=ac.size(), col=ac[0].size();
    std::size_t row=orow,start=0;
    // startより左は0
    // forwarding reduction
    for(std::size_t i=0;i<row;){
      if(ac[i][start]==0){
        // その行がすべて0のとき削除
        // その行が最後の列以外0のとき制約の矛盾
        // どちらでもないとき，他の行にその列を主成分に持つ行が存在するとき入れ替え
        // そのような行が無いときは主成分を一つ右にずらす．
        std::int64_t nonzero=-1;
        for(std::size_t j=start+1;j<col;++j)
          if(ac[i][j]!=0){
            nonzero=j;
            break;
          }
        if(nonzero==-1){
          if(i+1!=row) std::swap(ac[i],ac.back());
          ac.pop_back();
          --row;
          continue;
        }
        if(nonzero==col-1)
          throw std::runtime_error("GaussianReduction: there are constraint confliction.");
        bool empty=true;
        for(std::size_t ii=i+1;ii<row;++ii){
          if(ac[ii][start]==0) continue;
          std::swap(ac[ii],ac[i]);
          empty=false;
          break;
        }
        if(empty){
          ++start;
          continue;
        }
      }
			if(start+1==col) throw std::runtime_error("GaussianReduction: there are constraint confliction.");
      const T divk=1/ac[i][start];
      for(std::size_t j=start;j<col;++j)
        ac[i][j]*=divk;
      for(std::size_t ii=i+1;ii<row;++ii){
        if(ac[ii][start]==0)continue;
        const T mulk=ac[ii][start];
        for(std::size_t j=start;j<col;++j)
          ac[ii][j]-=ac[i][j]*mulk;
      }
      ++start,++i;
    }
    vc b(col-1,0);
    for(std::int64_t i=row-1;i>=0;--i){
			for(std::size_t j=0;j<col-1;++j)
				if(ac[i][j]!=0){
					start=j;
					break;
				}
      b[start]=ac[i].back();
      for(std::size_t ii=0;ii<i;++ii){
        const T subk=ac[ii][start];
        for(std::size_t j=start;j<col;++j)
          ac[ii][j]-=subk*ac[i][j];
      }
    }
    return b;
  }
};

} // namespace elsie