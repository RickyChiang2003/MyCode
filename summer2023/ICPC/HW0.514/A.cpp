#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
using namespace std;

string a[6];
char ans[30][30] = { 0 };
char tmp[30][30] = { 0 };
string ans_s[6];

int is_accepted(vector<int> idx) {
    if (a[idx[0]].front() == a[idx[5]].front())
    if (a[idx[0]].back() == a[idx[1]].front())
    if (a[idx[5]].back() == a[idx[4]].front())
    if (a[idx[4]].back() == a[idx[3]].front())
    if (a[idx[1]].back() == a[idx[2]].front())
    if (a[idx[2]].back() == a[idx[3]].back())
    if (a[idx[0]].length() + a[idx[2]].length() == a[idx[4]].length() + 1)
    if (a[idx[3]].length() + a[idx[5]].length() == a[idx[1]].length() + 1)
    if (a[idx[4]][a[idx[0]].length() - 1] == a[idx[1]][a[idx[5]].length() - 1])
        return 1;
    return 0;
}

int main(){
    for (auto& s : a) 
        cin >> s;
    for (int k = 0; k < 30; k++) {
        for (int i = 0; i < 30; i++)
            ans[k][i] = 'z';
    }
    
    int found = 0;
    int ans_len_c = 0, ans_len_r = 0;
    vector<int> idx = { 0,1,2,3,4,5 };
    while (next_permutation(idx.begin(), idx.end())) {
        if (is_accepted(idx)) {
            int len0 = a[idx[0]].length();
            int len2 = a[idx[2]].length();
            int len3 = a[idx[3]].length();
            int len5 = a[idx[5]].length();
            int len_c = a[idx[4]].length();
            int len_r = a[idx[1]].length();
            //cout << len0 << ' ' << len2 << ' ' << len3 << ' ' << len5 << ' ' << len_c << ' ' << len_r << endl;
            for (int i = 0; i < len_r; i++)
                for (int j = 0; j < len_c; j++)
                    tmp[i][j] = '.';
            for (int i = 0; i < len0; i++)
                tmp[0][i] = a[idx[0]][i];
            for (int i = 0, c = len0 - 1; i < len_r; i++)
                tmp[i][c] = a[idx[1]][i];
            for (int i = 0, c = len0 - 1; i < len2; i++)
                tmp[len_r - 1][i + c] = a[idx[2]][i];
            for (int i = 0, r = len5 - 1; i < len3; i++)
                tmp[i + r][len_c - 1] = a[idx[3]][i];
            for (int i = 0, r = len5 - 1; i < len_c; i++)
                tmp[r][i] = a[idx[4]][i];
            for (int i = 0; i < len5; i++)
                tmp[i][0] = a[idx[5]][i];

            int change = 0;
            for (int i = 0; i < len_r && change == 0; i++)
                for (int j = 0; j < len_c && change == 0; j++) {
                    if (j == ans_len_c - 1 && ans_len_c < len_c)
                        change = (ans[i][j] <= tmp[i][j]) ? 2 : 1;
                    else if (j == len_c - 1 && ans_len_c > len_c)
                        change = (ans[i][j] < tmp[i][j]) ? 2 : 1;
                    else if (ans[i][j] < tmp[i][j])
                        change = 2;
                    else if (ans[i][j] > tmp[i][j])
                        change = 1;
                }
            if (change == 1) {
                for (int i = 0; i < len_r; i++)
                    for (int j = 0; j < len_c; j++)
                        ans[i][j] = tmp[i][j];
                ans_len_c = len_c;
                ans_len_r = len_r;
            }

            found = 1;
        }        
    }

    if (!found)
        cout << "Impossible" << endl;
    else {
        for (int i = 0; i < ans_len_r; i++) {
            for (int j = 0; j < ans_len_c; j++)
                cout << ans[i][j];
            cout << endl;
        }
    }

    return 0;
}