#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
using namespace std;


# define DIM 12
# define N 256
# define init_x0 1
# define max_time 5*N
# define repeat_time 1000

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0,N-1);

    vector<vector<long double>> data;

		// init data : { x0 , x1 , ... , x12 , y }
		for (int i = 0; i < N; i++)
		{
			vector<long double> ex;
			ex.push_back(init_x0);
			for (int j = 1; j <= DIM + 1; j++)
			{
				long double input;
				cin >> input;
				ex.push_back(input);
			}
			data.push_back(ex);
		}

    int test[repeat_time] = {0};
    for(int cnt = 0; cnt < repeat_time; cnt++){
	vector<long double> w(13,0);
	int ac_cnt = 0;
	int change_cnt = 0;

	while(ac_cnt <= max_time){
	    int id = dist(gen);
	    int y = data[id][DIM+1];
	    long double product = 0;
	    for(int i = 0; i <= DIM; i++)
		product += (data[id][i])*(w[i]);
	    if(y*product <= 0){
//		cout<<cnt<<' '<<ac_cnt<<'\n';
		for(int i = 0; i <= DIM; i++)
		    w[i] += y*data[id][i];
		ac_cnt = 0;
		change_cnt += 1;
	    }
	    ac_cnt++;
	}
//	cout<<run_cnt<<'\n';
	test[cnt] = change_cnt;
    }
    
    sort(test, test+repeat_time);
    for(int i = 0; i < repeat_time; i++)
	cout<<test[i]<<'\n';

    return 0;
}
