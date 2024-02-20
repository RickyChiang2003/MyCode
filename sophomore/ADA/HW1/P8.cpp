#include <iostream>
#include <list>
using namespace std;
# define ll long long
# define SIZE 1000000

ll r[SIZE];
ll r_sum[SIZE];  // r_sum[m] = sum(r[0] ~ r[m])
list<ll> NLB;  // next possible lower bound

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int T;
    cin>>T;
    for(int cnt = 0; cnt < T; cnt++){
	// init N,K
	ll N,K;
	cin>>N>>K;
	for(int i = 0; i < N; i++){
	    cin>>r[i];
	    r_sum[i] = r[i];
	    if(i > 0)
		r_sum[i] += r_sum[i-1];
	}

	// O(N) find min_subarray that larger than K
	NLB.clear();
	ll min_dis = SIZE+1;
	for(int UB = 0; min_dis > 1 && UB < N; UB++){  // upper bound
	    if(!NLB.empty() && r_sum[UB]-r_sum[NLB.front()] >= K)
		while(!NLB.empty() && r_sum[UB]-r_sum[NLB.front()] >= K){
		    min_dis = min_dis < (UB-NLB.front()) ? min_dis : UB-NLB.front();
		    NLB.pop_front();
		}
	    if(!NLB.empty() && r_sum[UB] <= r_sum[NLB.back()])
		while(!NLB.empty() && r_sum[UB] <= r_sum[NLB.back()])
		    NLB.pop_back();
	    NLB.push_back(UB);

	    if(r_sum[UB] >= K && min_dis == SIZE+1)
		min_dis = min_dis < UB+1 ? min_dis : UB+1;
	}
	if(min_dis == SIZE+1)
	    cout<<-1<<'\n';
	else
	    cout<<min_dis<<'\n';
    }


    return 0;
}
