#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;
# define SIZE 150
# define ll long long

int main(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist_n(1,1500);
    uniform_int_distribution<> dist_K(0,500000);

    for(int cnt = 101; cnt <= SIZE; cnt++){
	int n = (int)dist_n(gen);
	int m = (int)dist_n(gen);
	int K = (int)dist_K(gen);
	uniform_int_distribution<> dist_small_n(1,n);
	uniform_int_distribution<> dist_small_m(1,m);
	uniform_int_distribution<> dist_large(0,1000000);

	string fn = "P9test_and_ans/P9test_";
	fn = fn + to_string(cnt);
	fn = fn + ".in";
	ofstream out(fn);
	out<<n<<' '<<m<<'\n';
	for(int i = 0; i < n; i++)
	    for(int j = 0; j < m; j++){
		out<<dist_large(gen);
		if(j == m-1)
		    out<<'\n';
		else
		    out<<' ';
	    }
	out<<K<<'\n';
	for(int i = 0; i < K; i++){
	    out<<dist_small_n(gen)<<' '<<dist_small_m(gen)<<' '<<0<<' '<<dist_large(gen)<<'\n';
	}
	out.close();

    }

    return 0;
}
