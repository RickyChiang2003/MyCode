#include <iostream>
#include <fstream>
#include <algorithm>
#include <climits>
#include <random>
using namespace std;
# define ll long long
# define N_MAX 300001
# define M_MAX 300001
// u,v = {1 ~ N}



int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(2,30000);

    ofstream out("P17.in");
    out<<"300000 300000\n";
    /*
    for(int i = 0; i < 299998; i++){
	out<<i+1<<' '<<i+2<<'\n';
    }
    out<<299998<<' '<<1<<'\n';
    out<<6<<' '<<8<<'\n';
    */
    for(int i = 0; i < 100; i++){
	out<<"1 "<<i+2<<'\n';
    }
    for(int i = 100; i < 300000; i++){
	int a,b;
	a = dist(gen);
	b = dist(gen);
	while(a == b)
	    b = dist(gen);
	out<<a<<' '<<b<<" 1\n";
    }
    out.close();
    

    return 0;
}
