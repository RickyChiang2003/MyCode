#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;
# define ll long long
# define N_MAX 5003 

// consider the two USB are A and B
ll xy[N_MAX][2] = {0};      // xy[i] means point_i for all i=[1,N]
ll dp[N_MAX][N_MAX] = {0};  // dp[i][j] means when A is at point_i and B is at point_j

ll dist(int i, int j){
    ll x1 = xy[i][0], y1 = xy[i][1], x2 = xy[j][0], y2 = xy[j][1];
    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    dp[0][0] = 0;
    xy[0][0] = xy[0][1] = 0;

    int N, T; 
    cin >> T;
    for(int cnt = 0; cnt < T; cnt++){
	cin >> N;
	for(int i = 1; i <= N; i++)
	    cin >> xy[i][0] >> xy[i][1];
	
	for(int i = 1; i <= N; i++)
	    dp[0][i] = dp[i][0] = dp[i-1][0]+dist(i,i-1);
	
	for(int j = 1; j <= N; j++){
	    for(int i = j+1; i <= N; i++){
		if(i == j+1){
		    ll tmp = LLONG_MAX;
		    for(int k = 0; k < j; k++)
			tmp = min(dp[k][j]+dist(i,k), tmp);
		    dp[j][i] = dp[i][j] = tmp;
		}
		else
		    dp[j][i] = dp[i][j] = dp[i-1][j] + dist(i,i-1);
	    }
	}

	ll ans = LLONG_MAX;
	for(int i = 0; i < N; i++){
	    ll tmp = dp[N][i] + dist(N,0) + dist(i,0);
	    if(tmp < ans)
		ans = tmp;
	}

	cout << ans << '\n';
    }


    return 0;
}
