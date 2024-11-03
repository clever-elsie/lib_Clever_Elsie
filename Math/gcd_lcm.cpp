
int gcd_(int a,int b){
	int r=1;
	if(a<0)a=-a;
	if(b<0)b=-b;
	while(r) r=a%b,a=b,b=r;
	return a;
}
int lcm_(int a,int b){ return a/gcd_(a,b)*b; }