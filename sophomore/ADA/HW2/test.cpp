#include <bits/stdc++.h>
using namespace std;

int main(){
    int n=10;
    int k=3;
    int l=4;
    // max = 25
    // int happy[n+1] = {0, 10, 4, 7, 3, 2, 5, 8, 1, 9, 6};
    // max = 27
    // int happy[n+1] = {0, 2, 4, 7, 3, 10, 5, 8, 1, 9, 6};
    int happy[n+1] = {0, 2, 3, 4, 7, 8, 9, 10, 6, 5, 1};

    int dp[n+1][k+1];
    for(int i=0;i<=n;i++){
        for(int j=0;j<=k;j++){
            dp[i][j] = 0;
        }
    }

    for(int j=1;j<=k;j++){
        deque<int> pre_max;
        for(int i=1;i<=n;i++){
            if(!pre_max.empty() && pre_max.back() < i-l){
                pre_max.pop_back();
            }
            while(!pre_max.empty() && dp[pre_max.front()][j-1] < dp[i-1][j-1]){
                pre_max.pop_front();
            }
            pre_max.push_front(i-1);
            dp[i][j] = dp[pre_max.back()][j-1] + happy[i];
        }
    }
    int ans = 0;
    for(int i=n-l+1;i<=n;i++) ans = max(ans, dp[i][k]);
    cout<<"maximum happiness = "<<ans<<"\n";
    for(int j=0;j<=k;j++){
        for(int i=0;i<=n;i++){
            cout<<dp[i][j]<<" ";
        }
        cout<<"\n";
    }
}
