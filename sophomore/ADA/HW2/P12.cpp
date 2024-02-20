#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;
# define ll long long
# define N_MAX 1000001 

ll suf_arr[N_MAX];

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int T,N;
    ll ans, pref_suf;
    cin >> T;
    for(int cnt = 0; cnt < T; cnt++){
	cin >> N;
	for(int i = 0; i < N; i++)
	    cin >> suf_arr[i];
	for(int i = N-2; i >= 0; i--)
	    suf_arr[i] += suf_arr[i+1];
	sort(suf_arr, suf_arr+N);
	
	ans = pref_suf = 0;
	for(int i = 0; i < (N>>1); i++){
	    pref_suf += suf_arr[N-1-i]-suf_arr[i];
	    if(ans <= pref_suf)
		ans = pref_suf;
	    else
		break;
	}

	cout << ans << '\n';
    }
    

    return 0;
}
