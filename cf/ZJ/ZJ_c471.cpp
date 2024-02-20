//#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>
using namespace std;

# define ll long long
# define SIZE 100000


bool cmp(pair<ll,ll> a, pair<ll,ll> b){
    return (a.first * b.second < b.first * a.second);
}


int main(){
    int N; cin >> N;
    vector<pair<ll, ll>> thing(N); //first for w and second for f
    for(int i = 0; i < N; i++) cin >> thing[i].first;
    for(int i = 0; i < N; i++) cin >> thing[i].second;

    sort(thing.begin(), thing.end(), cmp);

    ll t = 0, sum = 0;
    for(int i = N-1; i >= 0; i--){
	sum += t * thing[i].first;
	t += thing[i].second;
    }	

    cout << sum << endl;    



    return 0;
}
