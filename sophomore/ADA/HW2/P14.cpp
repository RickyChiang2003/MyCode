#include <iostream>
#include <algorithm>
using namespace std;
# define ll long long
# define N_MAX 500001

struct KIWI{
    int c,s,i,t;
};

KIWI kiwi[N_MAX];  // { c_i, s_i, i, times to make the can empty }, i = [1~N]
int ans[N_MAX];

bool s_cmp(KIWI a, KIWI b){
    return (a.s < b.s || (a.s == b.s && a.c > b.c));
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    int T,N,g,s_gkiwi;
    cin>>T;
    for(int cnt = 0; cnt < T; cnt++){
	cin>>N>>g;
	for(int i = 0; i < N; i++)
	    cin>>kiwi[i].c;
	for(int i = 0; i < N; i++)
	    cin>>kiwi[i].s;
	for(int i = 0; i < N; i++)
	    kiwi[i].i = i;
	s_gkiwi = kiwi[g-1].s;
	sort(kiwi, kiwi+N, s_cmp);

	if(kiwi[0].s != 0){
	    for(int i = 0; i < N; i++)
		cout<<-1<<(i == N-1 ? '\n':' ');
	    continue;
	}
	
	int idx = 0;
	int times = 0;
	int cur_max_c = -1;
	int max_c = -1;
	int all_bad = 0;
	for(idx = 0; idx < N; idx++){
	    if(kiwi[idx].s != 0)
		break;
	    max_c = cur_max_c = max(kiwi[idx].c, cur_max_c);
	    kiwi[idx].t = times;
	}

	times++;
	for(; idx < N; idx++){
	    if(kiwi[idx].s > cur_max_c && kiwi[idx].s > max_c){
		all_bad = 1;
		break;
	    }
	    if(kiwi[idx].s > cur_max_c){
		cur_max_c = max_c;
		times++;
	    }
	    max_c = max(max_c, kiwi[idx].c);
	    kiwi[idx].t = times;
	}
	for(; idx < N; idx++){
	    kiwi[idx].t = -1;
	}
	
	for(int i = 0; i < N; i++){
	    if(kiwi[i].c < s_gkiwi || kiwi[i].t == -1)
		ans[kiwi[i].i] = -1;
	    else if(kiwi[i].i != g-1)
		ans[kiwi[i].i] = 1+kiwi[i].t;
	    else
		ans[kiwi[i].i] = 0;
	}	
	for(int i = 0; i < N; i++)
	    cout<<ans[i]<<(i == N-1 ? '\n' : ' ');
    }


    return 0;
}
