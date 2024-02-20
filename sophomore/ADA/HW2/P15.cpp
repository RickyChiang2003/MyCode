#include <iostream>
#include <algorithm>
#include <climits>
#include <cmath>
using namespace std;
# define ll long long
# define n_MAX 10000000

ll arr[3][n_MAX];
ll arr_k5[3][n_MAX][3];

void planttree(ll n, ll k);
void planttree_k5(ll n, ll k);

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll n,k;
    cin>>n>>k;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < n; j++){
            char input;
            cin>>input;
            arr[i][j] = (input == '#') ? 0 : 1;
        }
    if(k > 5)
        planttree(n,k);
    else
        planttree_k5(n,k);


    return 0;
}

void planttree(ll n, ll k){
    // find distance for three line
    ll dist[3];
    dist[0] = sqrt(k);
    if(dist[0]*dist[0] < k) dist[0]++;
    dist[1] = ((dist[0]-1)*(dist[0]-1)+1 < k) ? dist[0] : dist[0]-1;
    dist[2] = ((dist[0]-1)*(dist[0]-1)+4 < k) ? dist[0] : dist[0]-1;
    // prev[a][b], a means the j index you have below, b means k
    int prev[3][3] = {{(int)dist[0],(int)dist[1],(int)dist[2]},
                      {(int)dist[1],(int)dist[0],(int)dist[1]},
                      {(int)dist[2],(int)dist[1],(int)dist[0]}};

    // main greedy
    for(int i = 0; i < n; i++)
        for(int j = 0; j < 3; j++){
            if(arr[j][i] == 0)
                arr[j][i] = (i == 0) ? 0 : arr[j][i-1];
            else{
                ll tmpmax = 0;
                for(int k = 0; k < 3; k++){
                    int d = prev[j][k];
                    // ignore if it's out of arr
                    if(i-d >= 0)
                        tmpmax = (tmpmax > arr[k][i-d]) ? tmpmax : arr[k][i-d];
                }
                arr[j][i] = tmpmax+1;
            }
        }

    // output
    int ans = max({arr[0][n-1],arr[1][n-1],arr[2][n-1]});
    cout<<ans<<'\n';
}

void planttree_k5(ll n, ll k){
    // init arr_k5
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < n; j++)
            arr_k5[i][j][0] = arr[i][j];

    // init distance
    int prev[3][3] = {{3,2,1},{2,3,2},{1,2,3}};


    // pain greedy
    for(int i = 0; i < n; i++)
        for(int j = 0; j < 3; j++){
	    // '#' case
            if(arr_k5[j][i][0] == 0){
                ll tmpmax = (i == 0) ? 0 : max({arr_k5[j][i-1][0],arr_k5[j][i-1][1],arr_k5[j][i-1][2]});
                arr_k5[j][i][0] = tmpmax;
                arr_k5[j][i][1] = tmpmax;
                arr_k5[j][i][2] = tmpmax;
            }
	    // '.' case
            else{
                ll tmpmax[3] = {0};
                for(int k = 0; k < 3; k++){
                    int d = prev[j][k];
                    // ignore if it's out of arr
		    if(i-d >= 0){
			if(k == j || k == 1)
			    tmpmax[k] = max({tmpmax[k],arr_k5[k][i-d][0],arr_k5[k][i-d][1],arr_k5[k][i-d][2]});
			else
			    for(int l = 0; l < 3; l++){
				if(l != j || l == 1)
				    tmpmax[k] = max(tmpmax[k],arr_k5[k][i-d][l]);
				else if(i-3 >= 0){
				    ll extra = max({arr_k5[j][i-3][0],arr_k5[j][i-3][1],arr_k5[j][i-3][2]});
				    extra += arr[k][i-d];
				    tmpmax[k] = max(tmpmax[k],extra);
				}
			    }
		    }
                }
                arr_k5[j][i][0] = tmpmax[0]+1;
                arr_k5[j][i][1] = tmpmax[1]+1;
                arr_k5[j][i][2] = tmpmax[2]+1;
            }
        }

    // output
    int ans = 0;
    for(int i = 0; i < 3; i++)
        for(int k = 0; k < 3; k++)
            ans = (ans > arr_k5[i][n-1][k]) ? ans : arr_k5[i][n-1][k];
    cout<<ans<<'\n';


}
