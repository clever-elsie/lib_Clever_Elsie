
int gcd_(int a,int b){
	int r=1;
	if(a<0)a=-a;
	if(b<0)b=-b;
	if(a<b)swap(a,b);
	if(b==0) return a;
	while(r) r=a%b,a=b,b=r;
	return a;
}
int lcm_(int a,int b){ return a/gcd_(a,b)*b; }

int egcd(int a,int b,int&x,int&y){
	if(b==0){
		x=1,y=0;
		return a;
	}
	int d=egcd(b,a%b,y,x);
	y-=a/b * x;
	return d;
}