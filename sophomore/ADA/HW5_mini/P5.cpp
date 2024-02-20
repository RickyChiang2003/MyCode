#include <iostream>
#include <algorithm>
#include <list>
using namespace std;
# define ll long long
# define N_MAX 1000002
# define M_MAX 1000002

typedef struct dsu_for_kruskal {
    ll id;
    ll p;
    ll r;
} DSU_k;

typedef struct edge_t {
    ll a;
    ll b;
    ll c;
} edge_t;

DSU_k node[N_MAX];
edge_t edge[M_MAX];


void init_node_and_edge(ll N, ll M){
    for(ll i = 1; i <= N; i++)
	node[i] = {i,i,1};
    for(ll i = 0; i < M; i++){
	ll a, b, c;
	cin>>a>>b>>c;
	edge[i] = {a,b,c};
    }
}

bool cmp_k(edge_t& a, edge_t& b){
    return a.c < b.c;
}

ll FIND_SET_k(ll a){
    if(node[a].id != node[a].p)
	node[a].p = FIND_SET_k(node[a].p);
    return node[a].p;
}

void UNION_k(ll a, ll b){
    a = FIND_SET_k(a);
    b = FIND_SET_k(b);
    if(a == b)
	return;

    ll newrank = node[a].r + node[b].r;
    if(node[a].r >= node[b].r){
	node[b].p = node[a].id;
	node[a].r = newrank;
    }
    else{
	node[a].p = node[b].id;
	node[b].r = newrank;
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll N,M;
    cin>>N>>M;
    init_node_and_edge(N, M);
    sort(edge, edge+M, cmp_k);
    
    ll ans = 0;
    for(ll i = 0; i < M; i++)
	if(FIND_SET_k(edge[i].a) != FIND_SET_k(edge[i].b)){
	    ans += edge[i].c;
	    UNION_k(edge[i].a, edge[i].b);
	}

    cout<<ans<<'\n';


    return 0;
}
