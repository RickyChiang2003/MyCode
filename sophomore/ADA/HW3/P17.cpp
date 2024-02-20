#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include <climits>
using namespace std;
# define ll long long
# define N_MAX 300001
# define M_MAX 300001
// u,v = {1 ~ N}


typedef struct dijkstra_ll{
    ll dist;
    ll prev;
} dij_ll;


vector<list<pair<ll,ll>>> neighbor(N_MAX);  // {vertex,weight}
ll road[M_MAX][3];  // small,large,w
dij_ll G[N_MAX];  // {dist,prev}
ll family[N_MAX];  // the ancestor node which is 1's child
ll stpset[N_MAX];  // 0 for have not gone here

void dijkstra(ll N, ll M){
    priority_queue<pair<ll,ll>, vector<pair<ll,ll>>, greater<pair<ll,ll>>> pq;
    // {dist, current}
    pq.push({0, 1});
    for(ll i = 0; i < N+1; i++){
        G[i].dist = LLONG_MAX;
        G[i].prev = -1;
    }
    G[1].dist = 0;

    stpset[1] = 1;
    while(!pq.empty()){
        ll u = pq.top().second;  // current
        pq.pop();
        for(pair<ll,ll>& c : neighbor[u]){
            ll v = c.first, w = c.second;
            ll tmpdist = G[u].dist + w;
            if(tmpdist < G[v].dist){
                G[v].dist = tmpdist;
                G[v].prev = u;
		if(stpset[v] < 3){
		    stpset[v]++;
		    pq.push({tmpdist,v});
		}
            }
        }
    }
}


void findfamily(ll N, ll M, ll curr){
    if(G[curr].prev == -1)
        family[curr] = -1;
    else if(G[curr].prev == 1)
        family[curr] = curr;
    else if(family[G[curr].prev] > 0)
        family[curr] = family[G[curr].prev];
    else{
        findfamily(N,M,G[curr].prev);
        family[curr] = family[G[curr].prev];
    }
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll N,M;
    cin>>N>>M;


    for(ll i = 0; i < M; i++){
        ll u,v,w;
        cin>>u>>v>>w;
        neighbor[u].push_front({v,w});
        neighbor[v].push_front({u,w});
        road[i][0] = min(u,v);
        road[i][1] = max(u,v);
        road[i][2] = w;
    }


    dijkstra(N, M);

    for(ll i = 2; i <= N; i++)
        if(family[i] == 0)
            findfamily(N, M, i);

    ll mindist = LLONG_MAX;
    for(ll i = 0; i < M; i++){
        ll u = road[i][0];
        ll v = road[i][1];
        ll w = road[i][2];
        if(u == 1){
            if(G[v].prev != 1 && family[v] != -1)  // accepted
                mindist = min(mindist, w + G[v].dist);
        }
        else if(family[u] != -1 && family[v] != -1){
            if(family[u] != family[v])
                mindist = min(mindist, w + G[u].dist + G[v].dist);
        }
    }

    cout<<mindist<<'\n';

    return 0;
}
