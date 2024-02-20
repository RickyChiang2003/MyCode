#include <iostream>
#include <algorithm>
#include <list>
#include <queue>
#include <climits>
using namespace std;
# define ll long long
# define n_MAX 300001
# define m_MAX 300001
// n = {3 ~ 3e5}
// m = {1 ~ min(3e5, n*n)}
// s,t,u,v = {1 ~ n}
// infinity = n_MAX+1

list<ll> child[n_MAX];
list<ll> pi[n_MAX];
ll min_dist[n_MAX];
ll min_dist_rev[n_MAX];
ll stpset[n_MAX];
ll suf_arr[n_MAX];

void BFS(ll n, ll m, ll s, ll t){
    queue<ll> ready_q;
    ready_q.push(s);
    min_dist[s] = 0;
    stpset[s]++;

    while(!ready_q.empty()){
	ll u = ready_q.front();
	ll next_w = min_dist[u]+1;
	ready_q.pop();

	for(auto& c : child[u])
	    if(stpset[c] == 0){
		ready_q.push(c);
		min_dist[c] = next_w;
		stpset[c]++;
	    }
    }
}

void BFS_rev(ll n, ll m, ll s, ll t){
    queue<ll> ready_q;
    ready_q.push(t);
    min_dist_rev[t] = 0;
    stpset[t]++;

    while(!ready_q.empty()){
	ll u = ready_q.front();
	ll next_w = min_dist_rev[u]+1;
	ready_q.pop();

	for(auto& c : pi[u])
	    if(stpset[c] == 0){
		ready_q.push(c);
		min_dist_rev[c] = next_w;
		stpset[c]++;
	    }
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    // init
    ll n,m,s,t;
    cin>>n>>m>>s>>t;
    for(ll i = 0, u, v; i < m; i++){
	cin>>u>>v;
	child[u].push_back(v);
	pi[v].push_back(u);
    }
    for(ll i = 1; i <= n; i++)
	min_dist[i] = min_dist_rev[i] = n_MAX+1;  // infinity
    
    // min_dist[i] is the distance from s to i
    // min_dist_rev[i] is the distance from i to t
    // suf_arr[i] is amount of vertex with min_dist >= i

    // 1st BFS
    BFS(n,m,s,t);
    
    // 2nd BFS
    for(ll i = 0; i <= n; i++) 
	stpset[i] = 0;
    BFS_rev(n,m,s,t);

    // init suf_arr
    for(ll i = 1; i <= n; i++){
	ll idx = (min_dist[i] == n_MAX+1) ? n : min_dist[i];
	suf_arr[idx]++;
    }
    for(ll i = n-1; i >= 0; i--)
	suf_arr[i] += suf_arr[i+1];

    // compute ans
    ll ans = 0;
    if(min_dist[t] < n_MAX+1){  // general cases
	for(ll i = 1; i <= n; i++){
	    ll cur_dist = min_dist[t] - min_dist_rev[i];
	    ll count = 0;

	    for(auto& c : pi[i])
		if(min_dist[c] >= cur_dist-1)
		    count++;
	    ll idx = (cur_dist <= 0) ? 0 : cur_dist-1;
	    ans += suf_arr[idx] - count;
	}
    }
    else{  // s is not connected to t
	ll s_neighbor = 0;
	ll t_neighbor = 0;
	for(ll i = 1; i <= n; i++)
	    if(min_dist[i] < n_MAX+1)
		s_neighbor++;
	for(ll i = 1; i <= n; i++)
	    if(min_dist_rev[i] < n_MAX+1)
		t_neighbor++;
	ans = n*n - s_neighbor*t_neighbor - m;
    }
    
    cout<<ans<<'\n';
    
    
    return 0;
}
