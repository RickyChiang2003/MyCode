#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <climits>
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
    ll groupnum = 1;  // total group + 1
    for(ll i = order_idx-1; i >= 1; i--)
        if(stpset[order[i]] != 1){
            DFS2(N, M, order[i], groupnum);
            groupnum++;
        }


    // create DAG with groups
    set<ll> G_group[groupnum];  // every cur->other edge of the group
    set<ll> G_group_rev_c[groupnum];  // every other->cur edge of the group
    ll G_group_rev[groupnum];  // amount of other->cur edge of the group
    ll w_group[groupnum];  // weight of the group
    list<ll> start_group;  // start groups in DAG
    for(ll i = 0; i < groupnum; i++)
        w_group[i] = G_group_rev[i] = 0;

    for(ll i = 1; i <= N; i++){
        ll g = group[i];  // current group
        w_group[g]++;
        for(auto& it : G[i])
            if(group[it] != g){
                G_group[g].insert(group[it]);  // cur->other
		if(G_group_rev_c[group[it]].count(g) == 0){  // avoid re-produce
		    G_group_rev_c[group[it]].insert(g);
		    G_group_rev[group[it]]++;
		}
            }
    }

    // find starting groups of DAG
    for(ll i = 1; i < groupnum; i++)
        if(G_group_rev[i] == 0)
            start_group.push_back(i);

    // find best walking list
    queue<ll> ready_q;
    for(auto& s : start_group){
        // push ready groups into ready_queue
        for(auto& it : G_group[s]){
            G_group_rev[it]--;
            if(G_group_rev[it] == 0)
                ready_q.push(it);
        }


        if(!ready_q.empty())
        while(!ready_q.empty()){
            auto it = ready_q.front();
            ready_q.pop();

            ll tmpmax = 0;
            for(auto& it2 : G_group_rev_c[it])
                tmpmax = max(tmpmax,w_group[it2]);
            w_group[it] += tmpmax;

            for(auto& it2 : G_group[it]){
                G_group_rev[it2]--;
                if(G_group_rev[it2] == 0)
                    ready_q.push(it2);
            }

        }
    }

    ll ans = 0;
    for(ll i = 1; i < groupnum; i++)
        ans = max(ans, w_group[i]);
    cout<<ans<<'\n';


    return 0;
}
