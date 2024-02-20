#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
using namespace std;
# define ll long long
# define n_MAX 100001

// turn the problem into: 
// Find min distance_2 between n points on xy-plane, 
// the range of {x, y} are {0~n, 0~(g_0+g_1+...+g_n-1))}

vector<pair<ll,ll>> point(n_MAX);
ll min_ = LLONG_MAX;

bool cmp(pair<ll,ll> a, pair<ll,ll> b){
    return (a.second < b.second || (a.second == b.second && a.first < b.first));
}

ll Dis2(pair<ll,ll> a, pair<ll,ll> b){
    return (a.first-b.first)*(a.first-b.first)+(a.second-b.second)*(a.second-b.second);
}

ll Dis2_arr(ll* a, ll* b){
    return (a[0]-b[0])*(a[0]-b[0])+(a[1]-b[1])*(a[1]-b[1]);
}

ll Closest_Dis2(ll front, ll back, ll point_y[][2]){
    // back-front == 1 or 2
    if(back-front == 1)
        return Dis2(point[front],point[back]);
    if(back-front == 2)
        return min({ Dis2(point[front],point[back]),Dis2(point[front+1],point[back]),Dis2(point[front],point[front+1]) });

    // divide into (front,middle),(middle+1,back)
    ll middle = (front+back)>>1;
    ll point_left[middle-front+1][2];
    ll point_right[back-middle][2];
    ll len = back-front+1;
    for(ll i = 0,il = 0,ir = 0; i < len; i++){
        if(point_y[i][0] <= middle){
            point_left[il][0] = point_y[i][0]; 
	    point_left[il][1] = point_y[i][1];
	    il++;
	}
        else{
            point_right[ir][0] = point_y[i][0];
            point_right[ir][1] = point_y[i][1];
	    ir++;
	}
    }

    ll min_left = Closest_Dis2(front,middle,point_left);
    ll min_right = Closest_Dis2(1+middle,back,point_right);
    min_ = min({min_,min_left,min_right});

    ll tmp;
    ll d_tmp = (ll)(sqrt((double)(min_)));
    if(front < (tmp = middle-d_tmp+1))
        front = tmp;
    if(back > (tmp = middle+d_tmp-1))
        back = tmp;

    ll arr_y[len][2];
    ll arr_len = 0;
    for(ll i = 0; i < len; i++)
	if(point_y[i][0] <= back || point_y[i][0] >= front){
	    arr_y[arr_len][0] = point_y[i][0];
	    arr_y[arr_len][1] = point_y[i][1];
	    arr_len++;
	}

    for(ll i = 0; i < arr_len; i++)
        for(ll j = i+1; j <= i+7 && j < arr_len; j++)
            min_ = min(min_,Dis2_arr(arr_y[i],arr_y[j]));

    return min_;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll n;
    cin >> n;
    ll sum = 0, input = 0, len = n+1;
    vector<pair<ll,ll>> point_y(len);
    for(ll i = 0; i < len; i++){
        point[i] = {i, sum};
        point_y[i] = {i, sum};
        if(i < n){
            cin>>input;
            sum += input;
        }
    }

    sort(point_y.begin(), point_y.begin()+len, cmp);

    ll point_arr_y[len][2];
    for(int i = 0; i < len; i++)
	point_arr_y[i][0] = point_y[i].first,
	point_arr_y[i][1] = point_y[i].second;

    cout << Closest_Dis2(0,n,point_arr_y) << '\n';  // start and last id


    return 0;
}
