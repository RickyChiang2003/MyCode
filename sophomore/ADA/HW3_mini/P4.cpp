#include <iostream>
#include <algorithm>
#include <list>
using namespace std;
# define ll long long
# define N_MAX 1000002
# define M_MAX 1000002

list<ll> G[N_MAX];  // G[i] is list of vertex_i's next point
list<ll> G_rev[N_MAX];  // reverse G[i]
ll stpset[N_MAX];  // stpset[i] means i'th point have been used or not
ll group[N_MAX];  // group[i] means the i'th point's groupnum
ll order[N_MAX];  // order[i] means the i'th leaved point from DFS
ll order_idx = 1;  // start from 1'st

void DFS(ll N, ll M, ll cur){
    if(stpset[cur] == 1) return;
    stpset[cur] = 1;

    for(auto& it : G[cur])
        DFS(N, M, it);

    order[order_idx] = cur;
    order_idx++;
}

void DFS2(ll N, ll M, ll cur, ll groupnum){
    if(stpset[cur] == 1) return;
    stpset[cur] = 1;

    for(auto& it : G_rev[cur])
        DFS2(N, M, it, groupnum);

    group[cur] = groupnum;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    // general SCC Kosaraju-Sharir algorithm
    ll N,M;
    cin>>N>>M;
    for(ll i = 0; i < M; i++){
        ll u,v;
        cin>>u>>v;
        G[u].push_back(v);
        G_rev[v].push_back(u);
    }

    for(ll i = 1; i <= N; i++)
        if(stpset[i] != 1)
            DFS(N, M, i);

    for(ll i = 1; i <= N; i++)
        stpset[i] = 0;
    ll groupnum = 1;  // total group+1
    for(ll i = order_idx-1; i >= 1; i--)
        if(stpset[order[i]] != 1){
            DFS2(N, M, order[i], groupnum);
            groupnum++;
        }

    ll group_pnum[groupnum];
    for(auto& c: group_pnum)
	c = 0;
    for(auto& c: group)
	if(c > 0)
	    group_pnum[c]++;
    ll ans = 0;
    for(auto& c: group_pnum)
	if(c > 1)
	    ans += c*(c-1)/2;

    cout<<ans<<'\n';




    return 0;
}
