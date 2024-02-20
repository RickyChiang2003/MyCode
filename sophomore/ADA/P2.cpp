#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
# define SIZE 3001

int dp[SIZE][SIZE];
string X;
string Y;
int len_X;
int len_Y;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin>>X>>Y;
    len_X = X.length();
    len_Y = Y.length();

    int min;
    for(int i = 0; i <= len_X; i++)
        for(int j = 0; j <= len_Y; j++){
            if(i == 0)
                dp[i][j] = j;
            else if(j == 0)
                dp[i][j] = i;
            else{
                if(X[i-1] == Y[j-1])
                    dp[i][j] = dp[i-1][j-1];
                else{
                    min = dp[i-1][j-1];
                    if(min > dp[i-1][j])
                        min = dp[i-1][j];
                    if(min > dp[i][j-1])
                        min = dp[i][j-1];
                    dp[i][j] = min+1;
                }
            }
        }

    cout<<dp[len_X][len_Y]<<'\n';


    return 0;
}
