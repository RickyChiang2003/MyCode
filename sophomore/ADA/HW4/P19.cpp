#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;
# define ll long long
# define Q_MAX 500001
# define y_MAX 500000000000001
# define y_MIN 0

vector<ll> kiwibuf(Q_MAX);
multiset<ll> set_l, set_r;
ll mid_l, mid_r = y_MAX, mid_real;
ll len_l, len_r, len_real, tol_l, tol_r;


void error_exit(int a){
    cout<<"You fucked up, haiyaa~~~~~~~\n";
    exit(a);
}
void insert_l(ll x){
    set_l.insert(x);
    tol_l += x;
    len_l ++;
}
void insert_r(ll x){
    set_r.insert(x);
    tol_r += x;
    len_r ++;
}
void erase_l(ll x){
    set_l.erase(set_l.find(x));
    tol_l -= x;
    len_l --;
}
void erase_r(ll x){
    set_r.erase(set_r.find(x));
    tol_r -= x;
    len_r --;
}
void init_mid3(){
    mid_l = (len_l > 0) ? *set_l.rbegin() : mid_l;
    mid_r = (len_r > 0) ? *set_r.begin() : mid_r;
    if(len_l == len_r)
	mid_real = (mid_r == y_MAX) ? 0 : (mid_l + mid_r)/2;
    else
	mid_real = (len_l < len_r) ? mid_r : mid_l;
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    ll Q, x, y, k;
    cin>>Q;
    string operation;
    for(ll cnt = 0; cnt < Q; cnt++){
	init_mid3();  // set correct mid_l, mid_r, mid_real
	cin>>operation;

	if(operation == "kiwi"){
	    cin>>x;
	    if(len_l == len_r){
		if(x > mid_l)
		    insert_r(x);
		else
		    insert_l(x);
	    }
	    else if(len_l < len_r){
		if(x > mid_r){
		    insert_l(mid_r);
		    erase_r(mid_r);
		    insert_r(x);
		}
		else
		    insert_l(x);
	    }
	    else if(len_l > len_r){
		if(x < mid_l){
		    insert_r(mid_l);
		    erase_l(mid_l);
		    insert_l(x);
		}
		else
		    insert_r(x);
	    }
	}
	else if(operation == "wiki"){
	    cin>>y;
	    size_t count_l = set_l.count(y);
	    size_t count_r = set_r.count(y);
	    if(count_l && count_r){
		if(len_l < len_r)
		    erase_l(y);
		else
		    erase_r(y);
	    }
	    else if(count_l && !count_r){
		erase_l(y);
		if(len_l < len_r){
		    insert_l(mid_r);
		    erase_r(mid_r);
		}
	    }
	    else if(!count_l && count_r){
		erase_r(y);
		if(len_l > len_r){
		    insert_r(mid_l);
		    erase_l(mid_l);
		}
	    }
	    else
		error_exit(0);
	}
	else if(operation == "wiwi"){
	    ll ans = mid_real*(len_l-len_r)-(tol_l-tol_r);
	    cout<<ans<<'\n';
	}
	else if(operation == "kiki"){
	    len_real = len_l + len_r;
	    cin>>k;
	    if(k == len_real)
		continue;

	    if(k <= len_real/2){
		ll idx = 0;
		fill(kiwibuf.begin(), kiwibuf.begin()+k, 0);
		for(auto& i : set_l){
		    kiwibuf[idx] += i;
		    idx = (idx+1)%k;
		}
		for(auto& i : set_r){
		    kiwibuf[idx] += i;
		    idx = (idx+1)%k;
		}
		sort(kiwibuf.begin(), kiwibuf.begin()+k);

		len_l = len_r = tol_l = tol_r = 0;
		set_l.clear();
		set_r.clear();		
		for(ll i = 0; i < k; i++){
		    if(i < k/2)
			insert_l(kiwibuf[i]);
		    else
			insert_r(kiwibuf[i]);
		}
	    }
	    else{
		ll opt_k = len_real - k;
		fill(kiwibuf.begin(), kiwibuf.begin()+opt_k, 0);
		auto it_l = set_l.begin();
		auto it_r = set_r.end();
		for(ll i = 0; i < opt_k; i++){
		    it_r--;
		    kiwibuf[i] += (*it_l);
		    kiwibuf[opt_k-i-1] += (*it_r);
		    tol_l -= *it_l;
		    tol_r -= *it_r;
		    it_l++;
		}
		len_l -= opt_k;
		len_r -= opt_k;
		set_l.erase(set_l.begin(), it_l);
		set_r.erase(it_r, set_r.end());

		for(ll i = 0; i < opt_k; i++)
		    insert_r(kiwibuf[i]);
		
		ll opt_k2 = (len_l < len_r-opt_k) ? 
			    (opt_k/2)+(opt_k%2) : (opt_k/2);
		
		it_r = set_r.begin();
		for(ll i = 0; i < opt_k2; i++){
		    insert_l(*it_r);
		    tol_r -= *it_r;
		    it_r++;
		}
		len_r -= opt_k2;
		set_r.erase(set_r.begin(), it_r);
	    }
	}
	else
	    error_exit(0);
	/*
	cout<<"set_l: ";
	for(auto i: set_l)
	    cout<<i<<' ';cout<<'; ';
	cout<<"set_r: ";
	for(auto i: set_r)
	    cout<<i<<' ';cout<<'\n';
	*/
    }

    return 0;
}
