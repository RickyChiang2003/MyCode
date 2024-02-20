#include <iostream>
#include <list>
using namespace std;
# define ll long long
# define n_MAX 1501
# define m_MAX 1501

ll B[n_MAX][m_MAX];
ll col_pref[n_MAX+1][m_MAX+1];
ll raw_pref[n_MAX+1][m_MAX+1];
ll hieght[m_MAX] = {0};  // actually the x index of the bottom
ll max_col[n_MAX];

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    ll n,m,K;
    cin>>n>>m;

    // init B
    for(int i = 1; i <= n; i++)
	for(int j = 1; j <= m; j++){
	    ll a;
	    cin>>a;
	    B[i][j] = -a;
	}
    cin>>K;
    for(int i = 0; i < K; i++){
	ll x,y,d,r;
	cin>>x>>y>>d>>r;
	B[x-d][y-d] += r;
    }
    
    // init max_col
    for(int i = 1; i <= n; i++){
	max_col[i] = m+1;
	ll max_pref = 0, cur_pref = 0;
	for(int j = m; j >= 1; j--){
	    cur_pref += B[i][j]; 
	    if(cur_pref > max_pref)
		max_col[i] = j, max_pref = cur_pref;
	}
    }

    // init col_pref
    for(int j = 1; j <= m; j++){
	col_pref[n+1][j] = 0;
	col_pref[n][j] = B[n][j];
	for(int i = n-1; i >= 1; i--)
	    col_pref[i][j] = col_pref[i+1][j] + B[i][j];
    }
    
    // init hieght
    for(int j = 1; j <= m; j++)
	hieght[j] = n+1;

    // produce
    for(int i = n; i >= 1; i--){
	ll max_flag = m+1;
	ll max_add = 0;
	ll pre_add = 0;
	for(int j = m; j >= max_col[i]; j--)
	    if(hieght[j] > i){
		pre_add += col_pref[i][j] - col_pref[hieght[j]][j];
		if(pre_add > max_add)
		    max_flag = j, max_add = pre_add;
	    }
	for(int j = max_flag; j <= m; j++){
	    if(hieght[j] > i)
		hieght[j] = i;
	    else
		break;
	}
    }

    ll ans = 0;
    for(int j = 1; j <= m; j++)
	ans += col_pref[hieght[j]][j];
    cout<<ans<<'\n';


    return 0;
}

