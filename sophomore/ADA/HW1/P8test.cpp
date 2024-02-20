#include <iostream>
#include <fstream>
#include <random>
using namespace std;
# define ll long long
# define SIZE 1000000
# define K 1

ll arr[SIZE] = {0};
ll sum[SIZE] = {0};

int main(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(-1000,5000);
    // case 1
    /*
    ofstream out("P8.in");
    out<<1<<'\n';
    out<<SIZE<<' '<<K<<'\n';
    for(int i = 0; i < SIZE-1; i++){
	arr[i] = dist(gen);
	sum[i] = (i == 0)? arr[i] : arr[i]+sum[i-1];
	out<<arr[i]<<' ';
    }
    arr[SIZE-1] = dist(gen);
    sum[SIZE-1] = sum[SIZE-2]+arr[SIZE-1];
    out<<arr[SIZE]<<'\n';
    out.close();
*/
    // case 2
    ofstream out("P8.in");
    out<<SIZE<<'\n';
    for(int i = 0; i < SIZE; i++){
	out<<1<<' '<<K<<'\n';
	out<<dist(gen)<<'\n';
    }
    out.close();


   /* 
    int min_dis = SIZE+1,low,high;
    for(int i = 0; i < SIZE && min_dis > 1; i++)
	for(int j = i; j < SIZE && min_dis > 1; j++)
	    if(sum[j]-sum[i]+arr[i] >= K && j-i+1<min_dis)
		min_dis = j-i+1,low=i,high=j;

    ofstream out_ans("P8.out");
    if(min_dis == SIZE+1)
	out_ans<<-1<<'\n';
    else
	out_ans<<min_dis<<' '<<low<<' '<<high<<'\n';
    out_ans.close();
*/
    return 0;
}
