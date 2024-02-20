//#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>
# define ll long long
using namespace std;


int N = 0;
ll L = 0, ans = 0;
stack<ll> stk, cnt;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N;

    //seek for the LHS houses in each turns, the while loop kill those houses that won't be found again, and the last line shows that there exist a taller house that could still be found

    for(int i = 0; i < N; i++){
	cin >> L;
	while(!stk.empty() && stk.top() < L){
	    ans += cnt.top();
	    stk.pop();
	    cnt.pop();
	}
	if(!stk.empty() && stk.top() == L){
	    ans += cnt.top();
	    cnt.top()++;
	}
	else{
	    stk.push(L);
	    cnt.push(1);
	}
	if(stk.size() > 1) ans++;
    }
    
    ans *= 2;
    cout << ans << endl;

    return 0;
}
