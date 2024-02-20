//#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, K; cin >> N >> K;
    vector<int> mates[1000005];
    for(int i = 0; i < K; i++){
	int p1, p2;
	cin >> p1 >> p2;
	mates[p1].emplace_back(p2);
	mates[p2].emplace_back(p1);
    }
    for(int i = 1; i <= N; i++){
	sort(mates[i].begin(), mates[i].end());
	int l = (int)mates[i].size();
	for(int j = 0; j < l; j++){
	    if(j != 0) cout << ' ';
	    cout << mates[i][j];
	}
	cout << endl;
    }
    

    return 0;
}
