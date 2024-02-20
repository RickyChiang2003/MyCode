//#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
# define _CRT_SECURE_NO_WARNINGS
# define _USE_MATH_DEFINES
#include <bits/stdc++.h>
# define ll long long
using namespace std;


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    string text1 = "abcde";
    string text2 = "acd";
    const int n1 = text1.length();
    const int n2 = text2.length();
    int dp[n1][n2];

    for (int i = 0; i < n1; i++)
        dp[i][0] = (text1[i] == text2[0]);
    for (int j = 0; j < n2; j++)
        dp[0][j] = (text1[0] == text2[j]);

    for (int i = 1; i < n1; i++)
        for (int j = 1; j < n2; j++)
        {
            if (text1[i] == text2[j])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
            cout << "dp[" << i << "][" << j << "] = " << dp[i][j] << endl;
        }

    cout << dp[n1 - 1][n2 - 1] << endl;

    return 0;
}

