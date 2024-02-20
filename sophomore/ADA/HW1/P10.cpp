#include <iostream>
#include <algorithm>
using namespace std;
# define ll long long
# define SIZE_N 100001
# define SIZE_K 31
# define SIZE_M 11


// turn the problem into: 
// Find min distance_2 between n points on xy-plane, 
// the range of {x, y} are {0~n, 0~(g_0+g_1+...+g_n-1))}

ll A[SIZE_N] = {0};
ll c[SIZE_K];
ll A_M[SIZE_M][SIZE_N];
ll subsum[SIZE_K][SIZE_M];  // deepness and remain


void init_A_M(ll N, ll M){
    for(ll i = 0; i < M; i++){
        A_M[i][0] = (A[0]+i)%M;
        for(ll j = 1; j < N; j++)
            A_M[i][j] = (A[j]+i)%M + A_M[i][j-1];
        A_M[i][N] = A_M[i][N-1];
    }
}

ll findsum(ll N, ll M, ll K, ll D, ll R){
    if(D == K)
        return (subsum[D][R] = A_M[R][N]);
    ll R2 = (R+c[D])%M;
    ll leftsum = (subsum[D+1][R] > -1) ? subsum[D+1][R] : findsum(N,M,K,D+1,R);
    ll rightsum = (subsum[D+1][R2] > -1) ? subsum[D+1][R2] : findsum(N,M,K,D+1,R2);
    return (subsum[D][R] = leftsum + rightsum);
}

ll BSfindsum(ll N, ll M, ll K, ll target, ll adjust_key){
    ll flag = target/N;
    ll sum = 0;
    ll D = 0, R = 0;
    ll low = 0, high = (1 << K)-1;
    ll mid;
    while(D < K){
        mid = (low + high)>>1;
        if(flag <= mid)
            high = mid;
        else{
            sum += subsum[D+1][R];
            R = (R+c[D])%M;
            low = mid+1;
        }
        D++;
    }
    ll flag2 = target%N-adjust_key;
    if(flag2 >= 0)
        sum += A_M[R][flag2];

    return sum;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll N,M,K,Q;
    cin>>N>>M>>K;
    for(ll i = 0; i < N; i++)
        cin>>A[i];
    for(ll i = 0; i < K; i++)
        cin>>c[i];

    init_A_M(N,M);
    for(ll i = 0; i <= K; i++)
        for(ll j = 0; j <= M; j++)
            subsum[i][j] = -1;
    subsum[0][0] = findsum(N, M, K, 0, 0);

    cin>>Q;
    ll l, r;
    for(ll cnt = 0; cnt < Q; cnt++){
        cin>>l>>r;
        ll leftsum = BSfindsum(N,M,K,l,1);
        ll rightsum = BSfindsum(N,M,K,r,0);
	cout<<(rightsum-leftsum)<<'\n';
    }


    return 0;
}
