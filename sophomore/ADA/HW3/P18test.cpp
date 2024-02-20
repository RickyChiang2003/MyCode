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

    ofstream out("P18.in");
    out<<"1000 5000\n";
    for(int i = 0; i < 1000; i++){
	int a,b;
	//a = dist(gen);
	//b = dist(gen);
	a = i+1;
	b = i+2;
	while(a == b)
	    b = dist(gen);
	out<<a<<' '<<b<<"\n";
    }
    for(int i = 1000; i < 5000; i++){
	int a,b;
	a = dist(gen);
	b = dist(gen);
	while(a == b)
	    b = dist(gen);
	out<<a<<' '<<b<<"\n";
    }
    out.close();
    

    return 0;
}
