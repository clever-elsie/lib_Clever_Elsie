#ifndef ELSIE_SPLAY_TREE
#define ELSIE_SPLAY_TREE
#include <cstddef>
#include <cstdint>
namespace elsie{
    using namespace std;
    template<class val_t>struct splay_node{
        using np=splay_node*;
        np p,ch[2];
        val_t val;
        size_t size;
        splay_node():size(0){}
        splay_node(np nil):size(0){ p=ch[0]=ch[1]=nil; }
        void set(const val_t&v){val=v;}
        void set(val_t&&v){val=move(v);}
    };
    template<class val_t>
    class splay_tree{
        using node=splay_node<val_t>;
        using np=node*;
        np root,nil;
        size_t size_;

        inline void calc_size(np x){
            if(x!=nil) x->size=1+x->ch[0]->size+x->ch[1]->size;
        }
        void rotate(np x,bool left_right){
            np p=x->p,g=p->p,a=x->ch[!left_right];
            x->p=g;
            if(g!=nil)g->ch[p==g->ch[1]]=x;
            x->ch[!left_right]=p;
            p->p=x,a->p=p;
            p->ch[left_right]=a;
            if(p==root)root=x;
            calc_size(p);
            calc_size(x);
        }
        void splay(np x){
            while(x!=root){
                np p=x->p;
                if(p==root){
                    rotate(x,x==p->ch[1]);
                }else{
                    np g=p->p;
                    bool pLR=p==g->ch[1],xLR=x==p->ch[1];
                    if(pLR==xLR){
                        rotate(p,pLR);
                        rotate(x,xLR);
                    }else{
                        rotate(x,xLR);
                        rotate(x,pLR);
                    }
                }
            }
        }
        public:
        struct iterator{
            np p;
            iterator(){}
            iterator(np x):p(x){}
        };
        splay_tree():size_(0){
            nil=new node();
            nil->p=nil->ch[0]=nil->ch[1]=nil;
            nil->size=0;
            root=nil;
        }
        iterator find(int64_t idx){
            if(idx<0)idx=size_+idx;
            if(idx<0||size_<=idx)return nil;
            np cur=root;
            while(cur!=nil){
                size_t L=cur->ch[0]->size;
                if(L>idx)cur=cur->ch[0];
                else if(L<idx){
                    cur=cur->ch[1];
                    idx-=L+1;
                }else{
                    splay(cur);
                    return iterator(cur);
                }
            }
            return iterator(nil);
        }
        iterator insert_at(int64_t idx,const val_t&v){
            if(size_==0){
                root=new node(nil);
                root->val=v;
                size_=1;
                return iterator(root);
            }
            iterator x=find(idx);
            np n=new node(nil);
            n->val=v;
            size_+=1;
            if(x.p==nil){
                np y=root;
                for(;y->ch[1]!=nil;y=y->ch[1])
                    y->size+=1;
                n->p=y,y->ch[1]=n;
            }else{
                n->ch[0]=x.p->ch[0];
                x.p->ch[0]->p=n;
                n->p=x.p,x.p->ch[0]=n;
            }
            splay(n);
            return iterator(n);
        }
        iterator erase_at(int64_t idx){
            if(size_==0)return iterator(nil);
            iterator x=find(idx);
            if(x.p==nil)return iterator(nil);
            else{ // root is equal to x.p
                if(size_==1){
                    size_=0;
                    delete root;
                    return iterator(root=nil);
                }
                size_-=1;
                if(root->ch[1]!=nil){
                    np y=root->ch[1];
                    while(y->ch[0]!=nil)
                        y->size-=1;
                    if(y!=root->ch[1]){
                        y->p->ch[0]=y->ch[1];
                        if(y->ch[1]!=nil) y->ch[1]->p=y->p;
                        y->ch[1]=root->ch[1];
                        root->ch[1]->p=y;
                    }
                    if(root->ch[0]!=nil) root->ch[0]->p=y;
                    y->ch[0]=root->ch[0];
                    y->p=nil;
                    calc_size(y);
                    delete root;
                    return iterator(root=y);
                }else{
                    np nr=root->ch[0];
                    nr->p=nil;
                    delete root;
                    return iterator(root=nr);
                }
            }
        }
        void bfs(){
            if(root==nil)return;
            deque<pair<np,int>>q(1,mp(root,0));
            vv<np>v(100);
            v[0].pb(root);
            while(q.size()){
                auto[p,d]=q.front();q.ppf();
                rep(i,2){
                    if(p->ch[i]!=nil){
                        q.eb(p->ch[i],d+1);
                        v[d+1].pb(p->ch[i]);
                    }
                }
            }
            cter(x,v)if(x.size()){
                cter(y,x)
                    print(y);
                    cerr<<sep;
            }
            NL;
        }
        void print(np y){
            cerr<<"("<<y->val<<" "<<y->size<<" "<<y->p->val<<" "<<y->ch[0]->val<<" "<<y->ch[1]->val<<") ";
        }
        size_t size(){ return size_; }
    };
}
#endif