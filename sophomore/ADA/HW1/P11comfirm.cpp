#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <climits>

using namespace std;
# define ll long long
# define n_MAX 100001

// turn the problem into: 
// Find min distance_2 between n points on xy-plane, 
// the range of {x, y} are {0~n, 0~(g_0+g_1+...+g_n-1))}

vector<pair<ll,ll>> point(n_MAX);

bool cmp(pair<ll,ll> a, pair<ll,ll> b){
    return (a.second < b.second || (a.second == b.second && a.first < b.first));
}

ll Distance_2(pair<ll,ll> a, pair<ll,ll> b){
    ll c = (a.first-b.first)*(a.first-b.first)+(a.second-b.second)*(a.second-b.second);
    return c;
}

ll Distance_2_arr(ll* a, ll* b){
    ll c = (a[0]-b[0])*(a[0]-b[0])+(a[1]-b[1])*(a[1]-b[1]);
    return c;
}
ll Closest_Dist2(ll front, ll back){
    // back-front == 1 or 2
    if(back-front == 1)
	return Distance_2(point[front],point[back]);
    if(back-front == 2)
	return min({ Distance_2(point[front],point[back]),Distance_2(point[front+1],point[back]),Distance_2(point[front],point[front+1]) });
    
    // divide into (front,middle),(middle+1,back)
    ll middle = (front+back)/2;
    ll min_left = Closest_Dist2(front,middle);
    ll min_right = Closest_Dist2(1+middle,back);
    ll min_ = min(min_left,min_right);
    ll tmp;
    if(front < (tmp = middle-sqrtl(min_)+1))
	front = tmp;
    if(back > (tmp = middle+sqrtl(min_)-1))
	back = tmp;
    for(ll i = front; i <= middle; i++)
	for(ll j = middle+1; j <= back; j++){
	    min_ = min(min_,Distance_2(point[i],point[j]));
	}
    
    return min_;    
}
ll _min_ = LLONG_MAX;
ll Closest_Dist22(ll front, ll back, ll point_y[][2]){
    // back-front == 1 or 2
    if(back-front == 1)
        return Distance_2(point[front],point[back]);
    if(back-front == 2)
        return min({ Distance_2(point[front],point[back]),Distance_2(point[front+1],point[back]),Distance_2(point[front],point[front+1]) });

    // divide into (front,middle),(middle+1,back)
    ll middle = (front+back)>>1;
    ll point_left[middle-front+1][2];
    ll point_right[back-middle][2];
    ll len = back-front+1;
    for(ll i = 0,il = 0,ir = 0; i < len; i++){
        if(point_y[i][0] <= middle)
            point_left[il][0] = point_y[i][0], 
	    point_left[il][1] = point_y[i][1],
	    il++;
        else
            point_right[ir][0] = point_y[i][0],
            point_right[ir][1] = point_y[i][1],
	    ir++;
    }

    ll min_left = Closest_Dist22(front,middle,point_left);
    ll min_right = Closest_Dist22(1+middle,back,point_right);
    _min_ = min({_min_,min_left,min_right});
    ll tmp;
    //cout<<"front,back and min_:"<<front<<','<<back<<','<<min_<<'\n';
    ll d_tmp = (ll)(sqrt((double)(_min_)));
    if(front < (tmp = middle-d_tmp+1))
        front = tmp;
    if(back > (tmp = middle+d_tmp-1))
        back = tmp;

    
    ll arr_y[len][2];
    ll arr_len = 0;
    for(ll i = 0; i < len; i++)
	if(point_y[i][0] <= back || point_y[i][0] >= front)
	    arr_y[arr_len][0] = point_y[i][0],
	    arr_y[arr_len][1] = point_y[i][1],
	    arr_len++;
	    

    for(ll i = 0; i < arr_len; i++)
        for(ll j = i+1; j <= i+7 && j < arr_len; j++){
	    ll tmp = Distance_2_arr(arr_y[i],arr_y[j]);
            _min_ = _min_ < tmp ? _min_ : tmp;
        }

    return _min_;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(-10000,10000);


    ll ans = 0;
    ll ans2 = 0;
    ll last_arr[1000] = {0};
    while(ans == ans2){
	ll n;
	n = 1000;/////
	ll sum = 0, input = 0, len = n+1;
	vector<pair<ll,ll>> point_y(len);
	for(ll i = 0; i < len; i++){
	    point[i] = {i, sum};
	    point_y[i] = {i, sum};
	    if(i < n){
		input = last_arr[i] = dist(gen);
		sum += input;
	    }
	}

	sort(point_y.begin(), point_y.begin()+len, cmp);
	ll point_arr_y[len][2];
	for(int i = 0; i < len; i++)
	    point_arr_y[i][0] = point_y[i].first,
		point_arr_y[i][1] = point_y[i].second;

	_min_ = LLONG_MAX;
	ans = Closest_Dist2(0,n);  // start and last id
	ans2 = Closest_Dist22(0,n,point_arr_y);
    cout << ans << ' ' << ans2 << '\n';
    }

    ofstream out("P11.in");
    out<<1000<<'\n';
    for(int i = 0; i < 1000; i++)
	out<<last_arr[i]<<'\n';
    out.close();

    return 0;
}

