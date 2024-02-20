#include <iostream>
#include <climits>
using namespace std;
# define ll long long
# define n_MAX 1502
# define m_MAX 1502

ll B[n_MAX][m_MAX] = {0};
ll raw_suf[n_MAX][m_MAX] = {0};
ll block_suf[n_MAX][m_MAX] = {0};
ll dp[n_MAX] = {0};

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

    // init dp
    for(int i = 0; i <= n+1; i++)
	dp[i] = (i == n+1) ? 0 : LLONG_MIN;

    // raw suffix
    for(int i = n; i >= 1; i--)
	for(int j = m; j >= 1; j--){
	    if(j == m)
		raw_suf[i][j] = B[i][j];
	    else
		raw_suf[i][j] = B[i][j] + raw_suf[i][j+1];
	}

    // block suffix (sum of the right above blocks)
    for(int j = m; j >= 1; j--)
	for(int i = n+1; i >= 1; i--){
	    if(i == n+1)
		block_suf[i][j] = 0;
	    else
		block_suf[i][j] = block_suf[i+1][j] + raw_suf[i][j];
	}

    // produce
    ll ans = 0;
    for(int j = 1; j <= m; j++){
	int lowerbound = n+1;
	ll tmp;
	for(int i = n; i >= 1; i--){
	    if(B[i][j] > 0){
		for(int k = i+1; k <= lowerbound; k++){
		    if(dp[k] > LLONG_MIN){
			tmp = block_suf[i][j] - block_suf[k][j] + dp[k];
			ans = (ans < tmp) ? tmp : ans;
			dp[i] = (dp[i] < tmp) ? tmp : dp[i];
		    }    
		}
		lowerbound = i;
	    }
	}
    }

    cout<<ans<<'\n';


    return 0;
}
